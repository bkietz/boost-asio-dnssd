###############
# Test options
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIR})

# convenience macro for test definition
function(cxx_test test_name)
	add_executable(${test_name} "${test_name}.cpp")
	target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES} -pthread)
	add_test(${test_name} ${test_name} "--gtest_output=xml:${test_name}.xml")
endfunction(cxx_test)

cxx_test(basic)
target_link_libraries(basic -lboost_system -lavahi-client -lavahi-common)
