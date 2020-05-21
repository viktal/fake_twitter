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

TEST_F(UsersEndpointTest, Update) {
    Rest::Routes::Put(
        router, "/update",
        Rest::Routes::bind(&UsersEndpoint::update, usersEndpoint));
    serveThreaded();
    std::optional<std::string> name = "name";
    std::optional<std::string> avatar = "path";

    EXPECT_CALL(*repository, update(1, name, avatar)).Times(1);

    auto response = client->Put("/update?id=1&name=name&avatar=path", "",
                                "application/x-www-form-urlencoded");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);

    EXPECT_CALL(*repository, update(1, name, avatar)).Times(0);
    response = client->Put("/update?name=name&avatar=path", "",
                           "application/x-www-form-urlencoded");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    name.reset();
    avatar.reset();
    EXPECT_CALL(*repository, update(1, name, avatar)).Times(1);
    response =
        client->Put("/update?id=1", "", "application/x-www-form-urlencoded");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);
}

TEST_F(UsersEndpointTest, Drop) {
    Rest::Routes::Delete(
        router, "/drop",
        Rest::Routes::bind(&UsersEndpoint::drop, usersEndpoint));
    serveThreaded();

    EXPECT_CALL(*repository, drop(1)).WillOnce(Return(true));
    auto response = client->Delete("/drop?id=1");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);

    EXPECT_CALL(*repository, drop(1)).WillOnce(Return(false));
    response = client->Delete("/drop?id=1");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);
}