add_subdirectory(googletest)
add_subdirectory(pistache)
set(HinnantDate_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/date)
add_subdirectory(sqlpp11-connector-sqlite3)
add_subdirectory(sqlpp11)

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE rapidjson/include)
