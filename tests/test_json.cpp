#include "gtest/gtest.h"
#include "fake_twitter/serializer/json.h"
#include "fake_twitter/fake.h"

//using namespace Pistache;
using namespace fake_twitter;

TEST(test_serialiser, json_user) {
    model::User fake_user = {0, "", "user", 123, "path path", 0, 0};

    std::string json = "{\"id\":0,\"name\":\"\",\"username\":\"user\",\"followers_count\":0,\"friends_count\":0,\"avatar\":\"path path\",\"password_hash\":123}";
    std::string user = serialization::to_json(fake_user);

    EXPECT_EQ(user, json);
    EXPECT_EQ(serialization::from_json<model::User>(user), fake_user);
}
