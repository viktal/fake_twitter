#include <gtest/gtest.h>

#include "EndpointTest.h"
#include "RepositoryMocks.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"
#include "fake_twitter/fake.h"
#include "fake_twitter/serializer/json.h"
#include "fake_twitter/utils.h"

using namespace fake_twitter;
using namespace Pistache;
using fake_twitter::endpoints::UsersEndpoint;
using ::testing::Return;

class UsersEndpointTest : public EndpointTest {
protected:
    void SetUp() override {
        EndpointTest::SetUp();
        mockUsersRepository = std::make_shared<MockUsersRepository>();
        usersEndpoint = std::make_shared<endpoints::UsersEndpoint>(
            std::static_pointer_cast<repository::UsersRepository>(
                mockUsersRepository));
    }
    std::shared_ptr<MockUsersRepository> mockUsersRepository;
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
    EXPECT_CALL(*mockUsersRepository, get(1)).WillOnce([]() {
        return nullptr;
    });

    auto response = client->Get("/show?id=1");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    // Valid user ID
    auto user = fake::user::object();
    EXPECT_CALL(*mockUsersRepository, get(123)).WillOnce([&]() {
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
    EXPECT_CALL(*mockUsersRepository, create("name", "username", "1235"))
        .WillOnce(Return(user));

    auto response =
        client->Post("/create?name=name&username=username&password=1235", {});
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

    auto fakeuser = model::User{22, "name", "username", 123, "salt", 0, 0};
    auto headers = make_cookies(fakeuser);

    EXPECT_CALL(*mockUsersRepository,
                update(22, std::optional<std::string>{"habrahabr"}))
        .Times(1);

    auto response = client->Put("/update?id=22&name=habrahabr", headers, "",
                                "application/x-www-form-urlencoded");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);

    EXPECT_CALL(*mockUsersRepository, update(22, {"habrahabr"})).Times(0);
    response = client->Put("/update?name=habrahabr", headers, "",
                           "application/x-www-form-urlencoded");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    EXPECT_CALL(*mockUsersRepository, update(22, std::optional<std::string>{}))
        .Times(1);
    response = client->Put("/update?id=22", headers, "",
                           "application/x-www-form-urlencoded");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);

    EXPECT_CALL(*mockUsersRepository, update(5, {"habrahabr"})).Times(0);
    response = client->Put("/update?id=5&name=habrahabr", headers, "",
                           "application/x-www-form-urlencoded");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Forbidden);
}

TEST_F(UsersEndpointTest, Drop) {
    Rest::Routes::Delete(
        router, "/drop",
        Rest::Routes::bind(&UsersEndpoint::drop, usersEndpoint));
    serveThreaded();

    auto fakeuser = model::User{1, "name", "username", 123, "salt", 0, 0};
    auto headers = make_cookies(fakeuser);

    EXPECT_CALL(*mockUsersRepository, drop(1)).WillOnce(Return(true));
    auto response = client->Delete("/drop?id=1", headers);
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);

    EXPECT_CALL(*mockUsersRepository, drop(1)).WillOnce(Return(false));
    response = client->Delete("/drop?id=1", headers);
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    EXPECT_CALL(*mockUsersRepository, drop(5)).Times(0);
    response = client->Delete("/drop?id=5", headers);
    ASSERT_EQ(Http::Code(response->status), Http::Code::Forbidden);
}