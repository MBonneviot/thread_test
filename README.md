# thread_test
some code sample using thread in C++

## Prerequisite
What I installed
* clang 5.0
* CMake 3.9.4
* Ninja
* Boost 1.65.1
* Boost DI : https://github.com/boost-experimental/di

Download boost di header file: di.hpp, and place it in a directory di/include/boost/ on level upper your checkout;

What env variables required
* export CLANG_DIR
* export BOOST_DIR
* export LD_LIBRARY_PATH=$BOOST_DIR/lib
* export DYLD_LIBRARY_PATH=$BOOST_DIR/lib

## Build
```
cmake -G Ninja
ninja
```
