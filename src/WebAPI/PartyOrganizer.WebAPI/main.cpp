#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>
#include "symbols.hh"

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

				   // Define the API:
auto hello_api = http_api(

	// The hello world procedure.
	GET / _hello = []() { return D(_message = "Hello world."); }
);

int main()
{
	printf("Starting server...");
	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(hello_api, 12345);
}
