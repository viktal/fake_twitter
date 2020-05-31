#include "fake_twitter/fake.h"
#include "fake_twitter/serializer/json.h"
#include "gtest/gtest.h"

using namespace fake_twitter;

TEST(test_serialiser, json_user) {
    model::User fake_user = {0, "", "user", 123, "123", 0, 0};

    std::string json =
        "{\"id\":0,\"name\":\"\",\"username\":\"user\",\"followers_count\":0,"
        "\"friends_count\":0,\"password_hash\":123,\"salt\":\"123\"}";
    std::string user = serialization::to_json(fake_user);

    ASSERT_EQ(user, json);
    ASSERT_EQ(serialization::from_json<model::User>(user), fake_user);
}

TEST(test_serialiser, json_tweet) {
    auto time =
        date::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    model::Tweet fake_tweet = {0, "body", 10, time, 2, 3};

    std::stringstream builder;
    builder << R"({"id":0,"body":"body","author":10,"create_date":")"
            << date::format("%FT%TZ", time) << R"(","retweets":3,"rating":2})";
    std::string tweet = serialization::to_json(fake_tweet);

    ASSERT_EQ(tweet, builder.str());
    ASSERT_EQ(serialization::from_json<model::Tweet>(tweet), fake_tweet);
}
