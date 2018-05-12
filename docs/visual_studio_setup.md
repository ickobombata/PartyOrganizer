# Visual Studio on Linux setup

### Things to keep in mind that we need to do:
* make it use clang
* copy all header files to windows machine and add them to the project's VC++ directories in order to get intellisense
* add C++ compiler and linker options in visual studio settings (-lmicrohttpd, include paths, -std=c++14, etc...)
* bring up VS linux console - Debug -> Linux console
