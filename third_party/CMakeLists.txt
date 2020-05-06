add_subdirectory(googletest)
add_subdirectory(pistache)

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE rapidjson/include)

