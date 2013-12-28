project(boost-asio-dnssd CXX)

###############
# CMake options
cmake_minimum_required(VERSION 2.8)
include(CTest)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

###############
# C++ options
# Boost version 1.55.0
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
include_directories(${CMAKE_SOURCE_DIR})

###############
add_subdirectory(test)

###############
# Test options
set(GTESTLIBS -lgtest -lgmock -pthread)

# convenience macro for test definition
function(cxx_test test_name)
	add_executable(${test_name} "${test_name}.cpp")
	target_link_libraries(${test_name} ${GTESTLIBS})
	add_test(${test_name} ${test_name} "--gtest_output=xml:${test_name}.xml")
endfunction(cxx_test)
