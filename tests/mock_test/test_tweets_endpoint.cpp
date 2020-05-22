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
    auto tweet = fake::tweet::object(1);
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

    model::Tweet tweet = {0, "body", 1, "12.12.12", 0, 0};
    EXPECT_CALL(*mockTweetsRepository, create(1, "body"))
        .WillOnce(Return(tweet));

    auto response = client->Post("/create?author=1", "{\"body\":\"body\"}",
                                 "application/json");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);
    ASSERT_EQ(
        fake_twitter::serialization::from_json<model::Tweet>(response->body),
        tweet);

    response = client->Post("/create", {});
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);
}
TEST_F(TweetsEndpointTest, Drop) {
    Rest::Routes::Delete(
        router, "/drop",
        Rest::Routes::bind(&TweetsEndpoint::drop, tweetsEndpoint));
    serveThreaded();

    EXPECT_CALL(*mockTweetsRepository, drop(1)).WillOnce(Return(true));
    auto response = client->Delete("/drop?id=1");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Ok);

    EXPECT_CALL(*mockTweetsRepository, drop(1)).WillOnce(Return(false));
    response = client->Delete("/drop?id=1");
    ASSERT_EQ(Http::Code(response->status), Http::Code::Bad_Request);
}
