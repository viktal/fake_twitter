add_subdirectory(googletest)

set(PISTACHE_ENABLE_NETWORK_TESTS off)
set(PISTACHE_INSTALL off)
add_subdirectory(pistache)

# Proper pistache targets with public include
include(FindPkgConfig)
find_package(Threads REQUIRED)

add_library(mypistache SHARED $<TARGET_OBJECTS:pistache>)
target_link_libraries(mypistache PUBLIC Threads::Threads)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(mypistache PUBLIC gcov)
endif ()
target_include_directories(mypistache PUBLIC pistache/include)

add_library(pistache-test-utils INTERFACE)
target_include_directories(pistache-test-utils INTERFACE pistache/tests)

add_subdirectory(date)

# Workaround for connections pool in sqlpp11(it uses a feature that is available for mysql connector only)
execute_process(COMMAND sed -i "s/connection->is_valid()/true/g" ${CMAKE_CURRENT_SOURCE_DIR}/sqlpp11/include/sqlpp11/connection_pool.h)

# sqlpp build tests by default
set(ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(HinnantDate_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/date)
add_subdirectory(sqlpp11)
add_subdirectory(sqlpp11-connector-postgresql)

find_package(PostgreSQL REQUIRED)
add_library(my-sqlpp11-connector-postgresql INTERFACE)
# sqlpp11 doesnt have proper cmake targets with public include folder(again)
target_link_libraries(my-sqlpp11-connector-postgresql INTERFACE sqlpp11 sqlpp11-connector-postgresql ${PostgreSQL_LIBRARIES})
target_include_directories(my-sqlpp11-connector-postgresql INTERFACE sqlpp11-connector-postgresql/include ${PostgreSQL_INCLUDE_DIRS})

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE rapidjson/include)
