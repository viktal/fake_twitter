add_library(fake_twitter INTERFACE)
target_include_directories(fake_twitter INTERFACE include)

add_executable(app src/app.cpp)
#target_include_directories(app PRIVATE include)
target_compile_features(app PRIVATE cxx_std_17)
#target_compile_options(app PRIVATE -fconcepts)
target_link_libraries(app mypistache rapidjson sqlpp11 sqlpp11-connector-postgresql ${PostgreSQL_LIBRARIES} fake_twitter)

add_executable(fake_db src/fake_db.cpp)
target_compile_features(fake_db PRIVATE cxx_std_17)
target_include_directories(fake_db PRIVATE include ${sqlpp11_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS} )
#target_link_libraries(fake_db sqlpp11-connector-sqlite3)
target_link_libraries(fake_db sqlpp11 sqlpp11-connector-postgresql ${PostgreSQL_LIBRARIES})
