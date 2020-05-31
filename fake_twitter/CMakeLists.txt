set(SOURCES
        src/serializer/json.cpp
        src/repository/TweetsRepository.cpp
        src/repository/UsersRepository.cpp
        src/repository/CommentsRepository.cpp
        src/repository/TagsRepository.cpp
        src/repository/TagTweetRepository.cpp
        src/repository/NewsFeedRepository.cpp
        src/endpoint/UsersEndpoint.cpp
        src/endpoint/TweetsEndpoint.cpp
        src/endpoint/CommentsEndpoint.cpp
        src/endpoint/TagsEndpoint.cpp
        src/endpoint/TagTweetEndpoint.cpp
        src/endpoint/NewsFeedEndpoint.cpp
        src/model/User.cpp
        src/model/Tweet.cpp
        src/RestServer.cpp
        src/utils.cpp
        )

add_library(fake_twitter STATIC ${SOURCES})
target_include_directories(fake_twitter PUBLIC include)
target_compile_features(fake_twitter PRIVATE cxx_std_17)
target_link_libraries(fake_twitter PUBLIC mypistache rapidjson sqlpp11 my-sqlpp11-connector-postgresql date)

add_executable(app src/app.cpp)
target_compile_features(app PRIVATE cxx_std_17)
#target_compile_options(app PRIVATE -fconcepts)
target_link_libraries(app PRIVATE fake_twitter)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fakeshow.html ${CMAKE_CURRENT_BINARY_DIR}/fakeshow.html COPYONLY)

add_executable(fake_db src/fake_db.cpp)
target_compile_features(fake_db PRIVATE cxx_std_17)
target_link_libraries(fake_db PRIVATE fake_twitter sqlpp11 my-sqlpp11-connector-postgresql)
