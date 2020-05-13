add_executable(usersTest usersTest.cpp)
target_compile_options(usersTest PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(usersTest mypistache rapidjson sqlpp11 sqlpp11-connector-sqlite3 endpoints gmock_main gcov fake_twitter)
add_test(NAME usersTest COMMAND usersTest)

add_executable(test test.cpp)
target_compile_options(test PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(test mypistache rapidjson sqlpp11 sqlpp11-connector-sqlite3 gmock_main gcov fake_twitter)
add_test(NAME test COMMAND test)

add_executable(integration_test integration_test.cpp)
target_compile_options(integration_test PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(integration_test mypistache rapidjson sqlpp11 sqlpp11-connector-sqlite3 gtest_main gcov fake_twitter)
add_test(NAME integration_test COMMAND integration_test)
