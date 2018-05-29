#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>
#include "symbols.hh"
#include <cstdio>
#include <jwt/jwt_all.h>

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

				   // Define the API:
auto hello_api = http_api(

	// The hello world procedure.
	GET / _hello = []() { return D(_message = "Hello world."); }
);

void GenerateToken(const std::string& username, const std::string& password)
{
	using json = nlohmann::json;

	// Setup a signer
	HS256Validator signer("secret!");

	// Create the json payload that expires 01/01/2017 @ 12:00am (UTC)
	json payload = { { "sub", "subject" },{ "exp", 1483228800 } };

	// Let's encode the token to a string
	auto token = JWT::Encode(signer, payload);

	printf("Token %s\n", token.c_str());
}

int main()
{
	setbuf(stdout, NULL);
	printf("Starting server...");
	GenerateToken(std::string(), std::string());

	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(hello_api, 12345);
}
