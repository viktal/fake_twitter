#include <gtest/gtest.h>

#include "EndpointTest.h"
#include "RepositoryMocks.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"
#include "fake_twitter/fake.h"
#include "fake_twitter/serializer/json.h"

using namespace fake_twitter;
using namespace Pistache;
using fake_twitter::endpoints::UsersEndpoint;
using ::testing::Return;

class UsersEndpointTest : public EndpointTest {
protected:
    void SetUp() override {
        EndpointTest::SetUp();
        repository = std::make_shared<MockUsersRepository>();
        usersEndpoint = std::make_shared<endpoints::UsersEndpoint>(
            std::static_pointer_cast<repository::UsersRepository>(repository));
    }
    std::shared_ptr<MockUsersRepository> repository;
    std::shared_ptr<UsersEndpoint> usersEndpoint;
};

TEST_F(UsersEndpointTest, Get) {
    // Setup routes
    Rest::Routes::Get(router, "/show",
                      Rest::Routes::bind(&UsersEndpoint::show, usersEndpoint));
    serveThreaded();

    // Invalid user ID
    // Note: Unique ptr is a move-only object, to avoid headache with gmock and
    // compiler, it is better to use lambda here
    EXPECT_CALL(*repository, get(1)).WillOnce([]() { return nullptr; });

    auto response = client->Get("/show?id=1");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    // Valid user ID
    auto user = fake::user::object();
    EXPECT_CALL(*repository, get(123)).WillOnce([&]() {
        return std::make_unique<model::User>(user);
    });

    response = client->Get("/show?id=123");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);
    ASSERT_EQ(
        fake_twitter::serialization::from_json<model::User>(response->body),
        user);
}

TEST_F(UsersEndpointTest, Create) {
    Rest::Routes::Post(
        router, "/create",
        Rest::Routes::bind(&UsersEndpoint::create, usersEndpoint));
    serveThreaded();

    model::User user = {0, "name", "username", 123};
    EXPECT_CALL(*repository, create("name", "username")).WillOnce(Return(user));

    auto response = client->Post("/create?name=name&username=username", {});
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);
    ASSERT_EQ(
        fake_twitter::serialization::from_json<model::User>(response->body),
        user);

    response = client->Post("/create", {});
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);
}

// TEST_F(UsersEndpointTest, Update) {
//    Rest::Routes::Post(
//        router, "/update",
//        Rest::Routes::bind(&UsersEndpoint::update, usersEndpoint));
//    serveThreaded();
//
//    model::User user = {0, "name", "username", 123};
////    EXPECT_CALL(*repository, update(1, "", "")).WillOnce());
//}