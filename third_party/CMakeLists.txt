add_subdirectory(googletest)

add_subdirectory(pistache)
include(FindPkgConfig)
find_package(Threads REQUIRED)
add_library(mypistache SHARED $<TARGET_OBJECTS:pistache>)
target_link_libraries(mypistache Threads::Threads gcov)
target_include_directories(mypistache PUBLIC pistache/include)

set(HinnantDate_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/date)
add_subdirectory(sqlpp11-connector-sqlite3)
add_subdirectory(sqlpp11)

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE rapidjson/include)
