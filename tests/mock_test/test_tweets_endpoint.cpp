#include <gtest/gtest.h>

#include "EndpointTest.h"
#include "RepositoryMocks.h"
#include "fake_twitter/endpoint/TweetsEndpoint.h"
#include "fake_twitter/fake.h"
#include "fake_twitter/serializer/json.h"

using namespace fake_twitter;
using namespace Pistache;
using fake_twitter::endpoints::TweetsEndpoint;
using ::testing::Return;

class TweetsEndpointTest : public EndpointTest {
protected:
    void SetUp() override {
        EndpointTest::SetUp();
        mockTweetsRepository = std::make_shared<MockTweetsRepository>();
        tweetsEndpoint = std::make_shared<endpoints::TweetsEndpoint>(
            std::static_pointer_cast<repository::TweetsRepository>(
                mockTweetsRepository));
    }
    std::shared_ptr<MockTweetsRepository> mockTweetsRepository;
    std::shared_ptr<TweetsEndpoint> tweetsEndpoint;
};

TEST_F(TweetsEndpointTest, Get) {
    Rest::Routes::Get(
        router, "/show",
        Rest::Routes::bind(&TweetsEndpoint::show, tweetsEndpoint));
    serveThreaded();
    EXPECT_CALL(*mockTweetsRepository, get(1)).WillOnce([]() {
        return nullptr;
    });

    auto response = client->Get("/show?id=1");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    // Valid user ID
    auto tweet = fake::tweet_comment::object_tweet(1);
    EXPECT_CALL(*mockTweetsRepository, get(2)).WillOnce([&]() {
        return std::make_unique<model::Tweet>(tweet);
    });

    response = client->Get("/show?id=2");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);
    ASSERT_EQ(
        fake_twitter::serialization::from_json<model::Tweet>(response->body),
        tweet);
}

TEST_F(TweetsEndpointTest, Create) {
    Rest::Routes::Post(
        router, "/create",
        Rest::Routes::bind(&TweetsEndpoint::create, tweetsEndpoint));
    serveThreaded();

    auto fakeuser = model::User{1, "name", "username", 123, "salt", 0, 0};
    auto headers = make_cookies(fakeuser);

    model::Tweet tweet = {0,
                          "body",
                          1,
                          std::chrono::time_point_cast<std::chrono::seconds>(
                              std::chrono::system_clock::now()),
                          0,
                          0};
    EXPECT_CALL(*mockTweetsRepository, create(1, "body"))
        .WillOnce(Return(tweet));

    auto response = client->Post("/create?author=1", headers,
                                 "{\"body\":\"body\"}", "application/json");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);
    ASSERT_EQ(
        fake_twitter::serialization::from_json<model::Tweet>(response->body),
        tweet);

    response = client->Post("/create", {});
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    response = client->Post("/create?author=5", headers, "{\"body\":\"body\"}",
                            "application/json");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Forbidden);
}

TEST_F(TweetsEndpointTest, Drop) {
    Rest::Routes::Delete(
        router, "/drop",
        Rest::Routes::bind(&TweetsEndpoint::drop, tweetsEndpoint));
    serveThreaded();

    auto fakeuser = model::User{1, "name", "username", 123, "salt", 0, 0};
    auto headers = make_cookies(fakeuser);
    auto tweet =
        model::Tweet{0,
                     "body",
                     1,
                     std::chrono::time_point_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now()),
                     0,
                     0};
    EXPECT_CALL(*mockTweetsRepository, get(0)).WillOnce([&]() {
        return std::make_unique<model::Tweet>(tweet);
    });
    EXPECT_CALL(*mockTweetsRepository, drop(0)).WillOnce(Return(true));

    auto response = client->Delete("/drop?id=0", headers);
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);

    EXPECT_CALL(*mockTweetsRepository, get(0)).WillOnce([&]() {
        return std::make_unique<model::Tweet>(tweet);
    });
    EXPECT_CALL(*mockTweetsRepository, drop(0)).WillOnce(Return(false));
    response = client->Delete("/drop?id=0", headers);
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);

    auto tweet2 =
        model::Tweet{0,
                     "body",
                     2,
                     std::chrono::time_point_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now()),
                     0,
                     0};
    EXPECT_CALL(*mockTweetsRepository, get(0)).WillOnce([&]() {
        return std::make_unique<model::Tweet>(tweet2);
    });
    EXPECT_CALL(*mockTweetsRepository, drop(0)).Times(0);
    response = client->Delete("/drop?id=0", headers);
    ASSERT_EQ(Http::Code(response->status), Http::Code::Forbidden);
}
