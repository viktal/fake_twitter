add_executable(example src/example.cpp)
target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pistache/include/ include)
target_link_libraries(example pistache_shared rapidjson sqlpp11 sqlpp11-connector-sqlite3)

add_executable(fake_db src/fake_db.cpp)
target_include_directories(fake_db PRIVATE include)
target_link_libraries(fake_db sqlpp11-connector-sqlite3)

add_executable(tmp src/tmp.cpp)
target_include_directories(tmp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pistache/include/ include)
target_link_libraries(tmp pistache_shared rapidjson)
