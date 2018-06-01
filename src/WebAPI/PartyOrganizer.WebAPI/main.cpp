#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>
#include <silicon/middlewares/mysql_connection.hh>
#include "symbols.hh"
#include <cstdio>

#include "Services/TokenService.h"

TokenService tokenService("ubersecret");

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

typedef decltype(D(_id(_auto_increment, _primary_key) = int(),
	_username = std::string(),
	_user_password = std::string(),
	_alias = std::string()))
	user;

				   // Define the API:
auto hello_api = http_api(

	// The hello world procedure.
	GET / _hello = []() { return D(_message = "Hello world."); },

	// example of the database data retrieval
	GET / _username = [](auto p, mysql_connection& db) {
		std::string name;
		db("SELECT username from users where alias = ?")("Due") >> name;
		return D(_username = name);
	}

	POST / _token / _generate * post_parameters(_username, _password)
		= [](auto params) { return D(_token = tokenService.GenerateToken(params.username, params.password)); },

	POST / _token / _validate * post_parameters(_token)
		= [](auto params) { return D(_valid = tokenService.ValidateToken(params.token)); }
);


int main()
{
	setbuf(stdout, NULL);
		printf("Starting server...");

	auto middlewares = std::make_tuple(
		mysql_connection_factory("localhost", "kurendo", "kurendo", "party_organizer")
	);

	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(hello_api, middlewares, 12345);
}
