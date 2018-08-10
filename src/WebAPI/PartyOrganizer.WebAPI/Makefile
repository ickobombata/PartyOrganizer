CXX=clang++
CXXFLAGS=-std=c++14 -ftemplate-depth=1024 -I ./include
LDFLAGS=-lmicrohttpd -lmysqlclient -lcrypto -lpthread

src=$(wildcard *.cpp) $(wildcard ./*/*.cpp) $(wildcard ./*/*/*.cpp)

party:
	$(CXX) $(CXXFLAGS) $(src) $(LDFLAGS)
