###############
# Test options
set(GTESTLIBS -lgtest -pthread) #-lgmock
enable_testing()

# convenience macro for test definition
function(cxx_test test_name)
	add_executable(${test_name} "${test_name}.cpp")
	target_link_libraries(${test_name} ${GTESTLIBS})
	add_test(${test_name} ${test_name} "--gtest_output=xml:${test_name}.xml")
endfunction(cxx_test)

cxx_test(basic)
target_link_libraries(basic -lboost_system -lavahi-client -lavahi-common)
