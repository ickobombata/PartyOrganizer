# jwt-cpp setup


## To be fixed, other library is used now

https://github.com/pokowaka/jwt-cpp

* The library is built on Ubuntu 18.04 and is located in the [libs](https://github.com/ickobombata/PartyOrganizer/tree/master/src/WebAPI/PartyOrganizer.WebAPI/libs) folder of the project.
* To get it running, copy the **jwt** and **private** folders from the [include](https://github.com/ickobombata/PartyOrganizer/tree/master/src/WebAPI/PartyOrganizer.WebAPI/include) folder to ~/include/jwt on the remote linux machine.
* Visual Studio project settings are already adjusted to include the library to the project.

### Follow the instructions below **ONLY** if you need to rebuild it.
#### Follow the instructions on the jwt-cpp github page:

* Install dependencies

``` sudo apt-get install libssl-dev cmake lcov ```

* Build

``` mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make 
```

* Find the ```libjwtd.a``` file and copy it to your location

#### Visual Studio settings
TBD
