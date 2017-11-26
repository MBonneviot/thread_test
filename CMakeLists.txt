cmake_minimum_required(VERSION 3.7.2)

project(thread-test)

message("Using env varialbe CLANG_DIR: $ENV{CLANG_DIR}")
message("Using env varialbe BOOST_DIR: $ENV{BOOST_DIR}")

set(CMAKE_CXX_COMPILER $ENV{CLANG_DIR}/clang++)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BOOST_ROOT $ENV{BOOST_DIR})
find_package(Boost COMPONENTS unit_test_framework system thread REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(thread-test 
    boost_main.cpp 
    std_thread_tests.cpp
    asio_thread_tests.cpp
    ThreadPool.cpp)

target_link_libraries(thread-test 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY})
