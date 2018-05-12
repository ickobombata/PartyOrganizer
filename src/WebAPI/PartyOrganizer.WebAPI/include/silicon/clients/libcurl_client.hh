#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <map>
#include <curl/curl.h>

#include <iod/json.hh>
#include <iod/sio_utils.hh>
#include <silicon/symbols.hh>
#include <silicon/http_route.hh>
#include <silicon/utils.hh>

namespace sl
{
  using namespace iod;

  template <typename R>
  struct response_parser
  {
    static auto run(long response_code, const std::string& body)
    {
      std::string error_message;
      std::string body_200;
      if (response_code == 200)
        body_200 = body;
      else
        error_message = body;
      return D(s::_status = response_code, s::_error = error_message, s::_response = body_200);
    }
  };

  template <typename T>
  struct static_str_to_std_str_helper { typedef T type; };
  template <>
  struct static_str_to_std_str_helper<const char*> { typedef std::string type; };
  template <unsigned N>
  struct static_str_to_std_str_helper<const char[N]> { typedef std::string type; };

  template <typename A>
  auto static_str_to_std_str(A v)
  {
    typedef typename static_str_to_std_str_helper<std::remove_reference_t<decltype(v.value())>>::type
      new_value_type;
    return typename A::symbol_type::template variable_type<new_value_type>();
  }
  
  template <typename... T>
  struct response_parser<sio<T...>>
  {
    static auto run(long response_code, const std::string& body)
    {
      sio<decltype(static_str_to_std_str(std::declval<T>()))...> result;

      std::string error_message;
      if (response_code == 200)
        json_decode(result, body);
      else
        error_message = body;

      return D(
        s::_status = response_code,
        s::_response = result,
        s::_error = error_message);
    }
  };

  template <typename E>
  inline size_t curl_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
  template <typename E>
  inline size_t curl_read_callback(void *ptr, size_t size, size_t nmemb, void *stream);

  template <typename Encoding>
  struct libcurl_http_client
  {
    libcurl_http_client()
    {
      curl_global_init(CURL_GLOBAL_ALL);
      curl_ = curl_easy_init();
    }

    ~libcurl_http_client()
    {
      curl_easy_cleanup(curl_);      
    }

    libcurl_http_client& operator=(const libcurl_http_client&) = delete;

    void connect(const std::string& host, int port)
    {
      host_ = host;
      port_ = port;
    }

    template <typename R, typename S, typename A>
    auto remote_call(S route, const A& args)
    {
      struct curl_slist *headers_list = NULL;

      // Generate url.
      std::stringstream url_ss;
      url_ss << "http://" << host_ << ":" << port_;

      // Path with parameters
      foreach(route.path) | [&] (auto m)
      {
        static_if<is_symbol<decltype(m)>::value>(
          [&] (auto m) { url_ss << "/" << m.name(); },
          [&] (auto m) { url_ss << "/" << args[m.symbol()]; },
          m);
      };

      // Get params
      static_if<(!std::is_same<decltype(iod::intersect(args, route.get_params)), void>::value)>(
        [&] (auto args) {
          auto get_params = iod::intersect(args, route.get_params);
          bool first = true;
          foreach(get_params) | [&] (auto m) {
            if (first) url_ss << '?';
            else url_ss << "&";
            std::stringstream value_ss;
            value_ss << m.value();
            char* escaped = curl_easy_escape(curl_, value_ss.str().c_str(),
                                             value_ss.str().size());
            url_ss << m.symbol().name() << '=' << escaped;
            first = false;
            curl_free(escaped);
          };
        },
        [&] (auto /*args*/) {}, args);

      // Pass the url to libcurl.
      curl_easy_setopt(curl_, CURLOPT_URL, url_ss.str().c_str());

      // POST parameters.
      bool is_urlencoded = std::is_same<Encoding, _x_www_form_urlencoded_t>::value;
      std::string rq_body;
      if (is_urlencoded)
      { // urlencoded
        req_body_buffer_.str("");
        static_if<(route.post_params._size > 0 and A::_size > 0)>(
          [&] (auto args) {
            auto post_params = iod::intersect(args, route.post_params);
            std::stringstream post_stream;
            bool first = true;
            foreach(post_params) | [&] (auto m)
            {
              if (!first) post_stream << "&";
              post_stream << m.symbol_name() << "=";
              std::stringstream value_str;
              static_if<is_sio<std::decay_t<decltype(m.value())>>::value>(
                [&] (auto m) { value_str << json_encode(m.value()); },
                [&] (auto m) { value_str << m.value(); }, m);

              char* escaped = curl_easy_escape(curl_, value_str.str().c_str(), value_str.str().size());
              first = false;
              post_stream << escaped;
              curl_free(escaped);
            };
            rq_body = post_stream.str();
            req_body_buffer_.str(rq_body);
          },
          [&] (auto /*args*/) {}, args);
      }
      else // Json encoded
        static_if<(route.post_params._size > 0 and A::_size > 0)>(
          [&] (auto args) {
            auto post_params = iod::intersect(args, route.post_params);        
            rq_body = json_encode(post_params);

          }, [](auto /*args*/){}, args);

      // HTTP POST
      if (std::is_same<decltype(route.verb), http_post>::value)
      {
        curl_easy_setopt(curl_, CURLOPT_POST, 1);
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, rq_body.c_str());
        if (is_urlencoded)
          headers_list = curl_slist_append(headers_list, "Content-Type: application/x-www-form-urlencoded");
        else
          headers_list = curl_slist_append(headers_list, "Content-Type: application/json");
      }

      // HTTP GET
      if (std::is_same<decltype(route.verb), http_get>::value) curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1);

      // HTTP PUT
      if (std::is_same<decltype(route.verb), http_put>::value)
      {
        curl_easy_setopt(curl_, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl_, CURLOPT_READFUNCTION, curl_read_callback<Encoding>);
        curl_easy_setopt(curl_, CURLOPT_READDATA, this);
        headers_list = curl_slist_append(headers_list, "Content-Type: application/x-www-form-urlencoded");
      }

      // HTTP DELETE
      if (std::is_same<decltype(route.verb), http_delete>::value)
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "DELETE");

      // Cookies
      curl_easy_setopt(curl_, CURLOPT_COOKIEJAR, 0); // Enable cookies but do no write a cookiejar.

      body_buffer_.clear();
      curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curl_write_callback<Encoding>);
      curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
      
      curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_list);

      // Send the request.
      char errbuf[CURL_ERROR_SIZE];
      curl_easy_setopt(curl_, CURLOPT_ERRORBUFFER, errbuf);
      if (curl_easy_perform(curl_) != CURLE_OK)
      {
        std::stringstream errss;
        errss << "Libcurl error when sending request: " << errbuf;
        throw std::runtime_error(errss.str());
      }
      curl_slist_free_all(headers_list);
      // Read response code.
      long response_code;
      curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);

      // Decode result.
      return response_parser<R>::run(response_code, body_buffer_);
    }

    void read(char* ptr, int size)
    {
      body_buffer_.append(ptr, size);
    }

    size_t write(char* ptr, int size)
    {
      size_t ret = req_body_buffer_.sgetn(ptr, size);
      return ret;
    }
    
    CURL* curl_;
    std::map<std::string, std::string> cookies_;
    std::string body_buffer_;
    std::stringbuf req_body_buffer_;
    std::string host_;
    int port_;
  };

  template <typename C, typename R, typename Ret>
  struct generic_client_call
  {
    generic_client_call(C c, R r) : client(c), route(r) {}

    template <typename... A>
    auto operator()(A... call_args)
    {
      return client->template remote_call<Ret>(route, D(call_args...));
    }

    C client;
    R route;
  };

  template <typename E>
  inline size_t curl_read_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
  {
    libcurl_http_client<E>* client = (libcurl_http_client<E>*)userdata;
    return client->write((char*) ptr, size * nmemb);
  }
  
  template <typename E>
  size_t curl_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
  {
    libcurl_http_client<E>* client = (libcurl_http_client<E>*)userdata;
    client->read(ptr, size * nmemb);
    return size * nmemb;
  }
  
  template <typename Ret, typename C, typename R>
  auto create_client_call(C& c, R route)
  {
    return generic_client_call<C, R, Ret>(c, route);
  }

  template <typename R, typename M>
  struct client_method_with_root : public R, 
                                   public M
  {
    using R::operator();
    client_method_with_root(R root, M methods) : R(root), M(methods) {}
  };

  template <typename R, typename M>
  auto make_client_methods_with_root(R root, M methods)
  {
    return client_method_with_root<R, M>(root, methods);
  }

  template <typename S>
  decltype(auto) process_roots(S o)
  {
    auto res = sio_iterate(o, D(_root = int(), _methods = sio<>())) | [] (auto m, auto prev)
    {
      // if m.value() is a sio, recurse.
      return static_if<is_sio<decltype(m.value())>::value>(
        [&] (auto m) { return D(_root = prev.root,
                                _methods = cat(prev.methods,
                                               D(m.symbol() = process_roots(m.value()))));
        },

        // if not, process a leave.
        [&] (auto m) {
      
          // if m is root
          return static_if<std::is_same<decltype(m.symbol()), _silicon_root____t>::value>(
            [&] (auto m, auto prev) {
              return D(_root = m.value(), _methods = prev);
            },
            [&] (auto m, auto prev) {
              // else
              return D(_root = prev.root, _methods = cat(D(m), prev.methods));
            }, m, prev);
        }, m);
    };

    return static_if<std::is_same<std::decay_t<decltype(res.root)>, int>::value>(
      [] (auto res) {
        return res.methods;
      },
      [] (auto res) {
        return make_client_methods_with_root(res.root, res.methods);
      }, res);
  }
  
  template <typename C, typename A, typename PR>
  auto generate_client_methods(C& c, A api, PR parent_route)
  {
    static_assert(is_tuple<decltype(api)>::value, "api should be a tuple.");

    auto tu = foreach(api) | [&] (auto m) {

      return static_if<is_tuple<decltype(m.content)>::value>(

        [&] (auto m) { // If tuple, recursion.
          return generate_client_methods(c, m.content, parent_route.append(m.route));
        },

        [&] (auto m) { // Else, register the procedure.
          typedef std::remove_reference_t<decltype(m.content)> V;
          typedef typename V::function_type F;
          typename V::arguments_type arguments;

          auto route = m.route;
          auto cc = create_client_call<callable_return_type_t<F>>(c, route);
          auto st = filter_symbols_from_tuple(route.path);
          auto st2 = std::tuple_cat(std::make_tuple(http_verb_to_symbol(route.verb)), st);

          return static_if<(std::tuple_size<decltype(m.route.path)>() != 0)>(
            [&] () {
              return symbol_tuple_to_sio(&st2, cc);
            },
            [&] () {
              auto p = std::tuple_cat(st2, std::make_tuple(_silicon_root___));
              return symbol_tuple_to_sio(&p, cc);
            });
        }, m);

    };

    // If in the tuple tu, there is a method without a path, this is the root
    // and we must make it accessible via the operator() of the result object.
    auto tu2 = deep_merge_sios_in_tuple(tu);
    return process_roots(tu2);
  }

  /*! 
  ** 
  ** 
  ** @param api 
  ** @param host 
  ** @param port 
  ** @param opts Available options are:
  **         _post_encoding = _x_www_form_urlencoded: Set the post parameter to be decoded as urlencoded.
  **         _post_encoding = _json: Set the post parameter to be decoded as json (Default).

  ** 
  ** @return 
  */
  template <typename A, typename... O>
  auto libcurl_json_client(const A& api, std::string host, int port, O... opts)
  {
    auto options = D(opts...);

    using client_t = libcurl_http_client<std::decay_t<decltype(options.get(_post_encoding, _json))>>;
    std::shared_ptr<client_t> c(new client_t());

    c->connect(host, port);
    return generate_client_methods(c, api, http_route<>());
  }
}
