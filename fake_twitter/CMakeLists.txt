add_executable(example src/example.cpp)
add_executable(example_rapid src/example_rapid.cpp)
target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pistache/include/)
target_link_libraries(example pistache_shared rapidjson)

target_link_libraries(example_rapid rapidjson)
