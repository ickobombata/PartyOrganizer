#pragma once

#include <iostream>
#include <thread>
#include <memory>
#include <cstring>
#include <vector>
#include <lwan/lwan.h>

#include <iod/json.hh>
#include <iod/utils.hh>

#include <silicon/symbols.hh>
#include <silicon/utils.hh>
#include <silicon/error.hh>
#include <silicon/service.hh>
#include <silicon/response.hh>
#include <silicon/optional.hh>
#include <silicon/middlewares/tracking_cookie.hh>
#include <silicon/middlewares/get_parameters.hh>

namespace sl
{

  typedef ::lwan_request lwan_request_internal;
  typedef struct ::lwan_response lwan_response_internal;

  struct lwan_request
  {
    lwan_request_internal* lwan_req;
  };

  struct lwan_response
  {
    lwan_response(lwan_request_internal* _lwan_req, lwan_response_internal* _lwan_resp)
      : lwan_req(_lwan_req),
        lwan_resp(_lwan_resp),
        status(0),
        headers(NULL),
        headers_size(20),
        headers_cpt(0)
    {
      
      headers = (lwan_key_value*) coro_malloc(lwan_req->conn->coro, headers_size * sizeof(lwan_key_value));
      assert(headers != NULL);
      for (int i = 0; i < headers_size; i++)
      {
        headers[i].key = NULL;
        headers[i].value = NULL;
      }
      lwan_resp->headers = headers;
    }

    void add_header(std::string k, std::string v)
    {
      if (headers_cpt == headers_size)
      {
        int old_size = headers_size;
        lwan_key_value* old_headers = headers;

        headers_size *= 2;
        headers = (lwan_key_value*) coro_malloc(lwan_req->conn->coro, headers_size * sizeof(lwan_key_value));
        assert(headers != NULL);        
        memcpy(headers, old_headers, old_size * sizeof(lwan_key_value));

        for (int i = old_size; i < headers_size; i++)
        {
          headers[i].key = NULL;
          headers[i].value = NULL;
        }
        
      }

      assert(lwan_req);
      assert(lwan_req->conn);
      assert(lwan_req->conn->coro);
      headers[headers_cpt].key = coro_strdup(lwan_req->conn->coro, k.c_str());
      headers[headers_cpt].value = coro_strdup(lwan_req->conn->coro, v.c_str());
      headers_cpt++;
    }

    void add_cookie(std::string k, std::string v)
    {
      add_header("Set-Cookie", k + '=' + v + ";Path=/");
    }
    
    lwan_request_internal* lwan_req;
    lwan_response_internal* lwan_resp;
    int status;
    std::string body;

    lwan_key_value* headers;
    int headers_size;
    int headers_cpt;
  };
  
  struct lwan_json_service_utils
  {
    typedef lwan_request request_type;
    typedef lwan_response response_type;

    template <typename S, typename O, typename C>
    void decode_get_arguments(O& res, C* req) const
    {
      foreach(S()) | [&] (auto m)
      {
        const char* param = lwan_request_get_query_param(req->lwan_req, m.symbol().name());
        if (param)
        {
          try
          {
            res[m.symbol()] = boost::lexical_cast<std::decay_t<decltype(res[m.symbol()])>>(param);
          }
          catch (const std::exception& e)
          {
            throw error::bad_request(std::string("Error while decoding the GET parameter ") +
                                     m.symbol().name() + ": " + e.what());
          }
        }
        else
        {
          if(!m.attributes().has(_optional))
            throw std::runtime_error(std::string("Missing required GET parameter ") + m.symbol().name());
        }
      };
    }

    template <typename P, typename O, typename C>
    void decode_url_arguments(O& res, const C& url) const
    {
      if (!url[0])
        throw std::runtime_error("Cannot parse url arguments, empty url");

      int c = 0;

      P path;

      foreach(P()) | [&] (auto m)
      {
        c++;
        iod::static_if<is_symbol<decltype(m)>::value>(
          [&] (auto m2) {
            while (url[c] and url[c] != '/') c++;
          }, // skip.
          [&] (auto m2) {
            int s = c;
            while (url[c] and url[c] != '/') c++;
            if (s == c)
              throw std::runtime_error(std::string("Missing url parameter ") + m2.symbol_name());

            try
            {
              res[m2.symbol()] = boost::lexical_cast<std::decay_t<decltype(m2.value())>>
              (std::string(&url[s], c - s));
            }
            catch (const std::exception& e)
            {
              throw error::bad_request(std::string("Error while decoding the url parameter ") +
                                       m2.symbol().name() + " with value " + std::string(&url[s], c - s) + ": " + e.what());
            }
            
          }, m);
      };
    }



    template <typename T, typename... D>
    void decode_post_parameter(sio<D...>*, T& res, const char* s) const
    {
      json_decode<sio<D...>>(res, s);
    }


    template <typename T, typename D>
    void decode_post_parameter(std::vector<D>*, T& res, const char* s) const
    {
      // FIXME handle vectors. json_decode<std::vector<D>>(res, s);
    }
    
    template <typename T>
    void decode_post_parameter(json_string*, T& res, const char* s) const
    {
      json_decode<json_string>(res, s);
    }
    
    template <typename T, typename U>
    void decode_post_parameter(T*, U& res, const char* s) const
    {
      res = boost::lexical_cast<T>(s);
    }
    
    template <typename P, typename O>
    void decode_post_parameters(O& res, lwan_request_internal* r) const
    {
      foreach(P()) | [&] (auto m)
      {
        const char* value = lwan_request_get_post_param(r, m.symbol_name());

        if (value)
        {
          try
          {
            decode_post_parameter((std::decay_t<decltype(m.value())>*) 0,
                                             res[m.symbol()], value);
          }
          catch (const std::exception& e)
          {
            throw error::bad_request(std::string("Error while decoding the post parameter ") +
                                     m.symbol().name() + " with value '" + value + "': " + e.what());
          }
        }
        else
        {
          if (!m.attributes().has(_optional))
            throw std::runtime_error(std::string("Missing post parameter ") + m.symbol_name());
        }
      };
    }
    
    template <typename P, typename T>
    auto deserialize(request_type* r, P procedure, T& res) const
    {
      try
      {
        
        decode_url_arguments<typename P::path_type>(res, std::string("/") + std::string(r->lwan_req->url.value, r->lwan_req->url.len));
        decode_get_arguments<typename P::route_type::get_parameters_type>(res, r);

        decode_post_parameters<typename P::route_type::post_parameters_type>(res, r->lwan_req);
      }
      catch (const std::runtime_error& e)
      {
        throw error::bad_request("Error when decoding procedure arguments: ", e.what());
      }
      
    }

    void serialize2(response_type* r, const std::string res) const
    {
      r->status = 200;
      r->body = res;
    }

    void serialize2(response_type* r, const string_ref res) const
    {
      r->status = 200;
      r->body = std::string(&res.front(), res.size());
    }
    
    void serialize2(response_type* r, const char* res) const
    {
      r->status = 200;
      r->body = res;
    }
    
    template <typename T>
    auto serialize2(response_type* r, const T& res) const
    {
      r->status = 200;
      r->body = json_encode(res);
    }

    template <typename T>
    auto serialize(response_type* r, const T& res) const
    {
      serialize2(r, res);
    }

    template <typename T>
    auto serialize2(response_type* r, const response_<T>& res) const
    {
      serialize2(r, res.body);
      if (res.has(_content_type))
        r->lwan_resp->mime_type = &res.get(_content_type, string_ref(""))[0];
    }
    
  };
  


  struct lwan_session_cookie
  {
    
    inline tracking_cookie instantiate(lwan_request* req, lwan_response* resp)
    {
      std::string key = "sl_token";
      std::string token;
      const char* token_ = lwan_request_get_cookie(req->lwan_req, key.c_str());
      
      if (!token_)
      {
        token = generate_secret_tracking_id();
        resp->add_cookie(key, token);
      }
      else
      {
        token = token_;
      }

      return tracking_cookie(token);
    }

  };
  
  
  template <typename S>
  static lwan_http_status
  lwan_silicon_handler(lwan_request_internal *request,
                       lwan_response_internal *response, void *data)
  {
    auto& service = * (S*)data;

    std::string method;
    if (request->flags & REQUEST_METHOD_GET) method = "GET";
    if (request->flags & REQUEST_METHOD_POST) method = "POST";

    std::string url(request->url.value, request->url.len);
    lwan_request rq{request};
    lwan_response resp(request, response);

    try
    {
      service(std::string("/") + std::string(method) + "/" + url, &rq, &resp, request, response);
    }
    catch(const error::error& e)
    {
      resp.status = e.status();
      std::string m = e.what();
      resp.body = m.data();
    }
    catch(const std::runtime_error& e)
    {
      std::cout << e.what() << std::endl;
      resp.status = 500;
      resp.body = "Internal server error.";
    }

    if (!response->mime_type)
      response->mime_type = "text/plain";
    strbuf_set(response->buffer, resp.body.c_str(), resp.body.size());

    return lwan_http_status(resp.status);
  }

  template <typename S>
  struct lwan_ctx
  {

    inline lwan_ctx(::lwan* lwan, S* service)
      : thread_(nullptr),
        lwan_(lwan),
        service_(service)
      {
        sptr_ = std::shared_ptr<int>((int*)13, [this] (int*)
                                     {
                                       stop();
                                       delete lwan_;
                                       delete service_;                                       
                                     });
      }

    inline ~lwan_ctx() {
    }

    inline void wait() { thread_->join(); }

    inline void start()
    {
      stop();
      thread_ = new std::thread([this] { lwan_main_loop(lwan_); });
    }
    inline void stop()
    {
      if (thread_ != nullptr)
      {
        shutdown((int)lwan_->main_socket, SHUT_RDWR);
        close((int)lwan_->main_socket);
        lwan_shutdown(lwan_);
        thread_->join();
        delete thread_;
        lwan_ = nullptr;
        thread_ = nullptr;
      }
    }
    std::shared_ptr<int> sptr_;
    std::thread* thread_;
    ::lwan* lwan_;
    S* service_;
  };

  struct lwan_sess { int id; };
  
  template <typename A, typename M, typename... O>
  auto lwan_json_serve(const A& api, M middleware_factories,
                       int port, O&&... opts)
  {

    auto options = D(opts...);
    auto m2 = std::tuple_cat(std::make_tuple(lwan_session_cookie()),
                                             middleware_factories);
    
    using service_t = service<lwan_json_service_utils, decltype(m2),
                              lwan_request*, lwan_response*, lwan_request_internal*, lwan_response_internal*>;
    
    auto s = new service_t(api, m2);

    // This won't compile with GCC.
    // const lwan_url_map_t default_map[] = {
    //   { .prefix = "/", .handler = &lwan_silicon_handler<service_t>, .data = &s},
    //   { .prefix = NULL }
    // };


    lwan_url_map default_map[2];
    memset(default_map, 0, sizeof(default_map));
    default_map[1].prefix = NULL;
    default_map[0].prefix = "/";
    default_map[0].handler = &lwan_silicon_handler<service_t>;
    default_map[0].data = s;
           
    
    ::lwan* l = new ::lwan;
    lwan_config c;


    c = *lwan_get_default_config();
    std::stringstream listen_str;
    listen_str << "*:" << port;
    c.listener = strdup(listen_str.str().c_str());
    
    lwan_init_with_config(l, &c);
    lwan_set_url_map(l, default_map);

    auto ctx = std::make_shared<lwan_ctx<service_t>>(l, s);

    ctx->start();

    if (!options.has(_non_blocking))
    {
      ctx->wait();
      ctx->stop();
    }

    return ctx;
  }

  template <typename A, typename... O>
  auto lwan_json_serve(const A& api, int port, O&&... opts)
  {
    return lwan_json_serve(api, std::make_tuple(), port, opts...);
  }
  
}
