project(boost-asio-dnssd CXX)

###############
# CMake options
cmake_minimum_required(VERSION 2.8)
include(CTest)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

###############
# C++ options
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
find_package(Boost 1.54 REQUIRED) # for boost::asio::spawn
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

###############
# Test subdir
add_subdirectory(test)

