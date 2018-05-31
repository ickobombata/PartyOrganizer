#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>
#include "symbols.hh"
#include <cstdio>

#include "Services/TokenService.h"

TokenService tokenService("ubersecret");

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

				   // Define the API:
auto hello_api = http_api(

	// The hello world procedure.
	GET / _hello = []() { return D(_message = "Hello world."); },

	POST / _token / _generate * post_parameters(_username, _password)
		= [](auto params) { return D(_token = tokenService.GenerateToken(params.username, params.password)); },

	POST / _token / _validate * post_parameters(_token)
		= [](auto params) { return D(_valid = tokenService.ValidateToken(params.token)); }
);


int main()
{
	setbuf(stdout, NULL);
	printf("Starting server...");

	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(hello_api, 12345);
}
