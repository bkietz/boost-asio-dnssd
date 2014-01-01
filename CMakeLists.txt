project(boost-asio-dnssd CXX)

###############
# CMake options
cmake_minimum_required(VERSION 2.8)
include(CTest)

###############
# C++ options
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Boost 1.54 REQUIRED) # for boost::asio::spawn
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

###############
# Test subdir
add_subdirectory(test)

