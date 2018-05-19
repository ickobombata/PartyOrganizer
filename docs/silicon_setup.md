# Silicon framework setup
http://siliconframework.org

### Please refer to the tutorial is something is unclear - http://siliconframework.org/docs/getting_started.html

## 1. VM Setup
* Install VMWare workstation
* Install Ubuntu 18.04 
* Change your VM network type to *Bridged* so its address can be accessed from the windows machine
* (Optional) Setup uni-sofia.bg apt-get mirrors - http://ubuntu.uni-sofia.bg/ubuntu/

## 2. Prequisities setup
* install git

```sudo apt install git```
* clone silicon

```git clone https://github.com/matt-42/silicon.git```
* install boost

```sudo apt install libboost-all-dev```
* install libmicrohttpd

```sudo apt install libmicrohttpd-dev```
* install cmake

```sudo apt install cmake```
* install clang

```sudo apt install clang```

## 3. Build Silicon
* enter silicon directory
* ```mkdir output```
* ```./install.sh output```

## 4. Test 
* create a cpp file (e.g ```vi test.cpp```)
* use the following contents
```c++
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

```
* ```cd output```
* generate iod symbols

```./bin/iod_generate_symbols ./test.cpp ./symbols.hh```
* build

```clang++ ./test.cpp -std=c++14 -lmicrohttpd -I ./include -o test.out```
* run app

```./test.out```
* open http://localhost:12345/hello
* you should see a ```Hello world!``` message
