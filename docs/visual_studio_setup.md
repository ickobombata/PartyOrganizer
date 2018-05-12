# Visual Studio on Linux setup

### Done:
* make it use clang
* copy all header files to windows machine and add them to the project's VC++ directories in order to get intellisense
* add C++ compiler and linker options in visual studio settings (-lmicrohttpd, include paths, -std=c++14, etc...)
* bring up VS linux console - Debug -> Linux console

## How it's done
### 1. IntelliSense
* Copy the include directory from silicon to ```$(ProjectDir)```
* Add ```$(ProjectDir)\include``` to VC++ Directories

### 2. Compiler
* Set ```clang++``` as compiler in C++ settings
* Clear unused flags
* Add include path to compiler settings - e.g ```~/Repositories/silicon/party/include```
* Add the ```-fPIC``` flag
* Change the object file name in C++ settings

### 3. Linker
* Change linker in Linker settings to clang++
* Tell the linker to include libmicrohttpd - Add to linker options ```-lmicrohttpd```

### 4. Build
* When you try to build, VS will prompt you for credentials to the linux machine
