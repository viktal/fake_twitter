#include <pistache/client.h>
#include "gtest/gtest.h"
#include "fake_twitter/RestServer.h"
#include "fake_twitter/fake.h"

using namespace Pistache;
using namespace fake_twitter;

class test_fixture_for_users : public ::testing::Test {
public:
    std::unique_ptr<RestServer> server;
    std::unique_ptr<Http::Client> client;

    void SetUp() override {
        Port port(8888);
        Address addr(Ipv4::any(), port);

        auto opts = Http::Endpoint::options()
                .threads(4)
                .flags(Tcp::Options::ReusePort | Tcp::Options::ReuseAddr);

        sql::connection_config config;
//        config.path_to_database = ":memory:";
        config.path_to_database = ":memory:";
        config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_TEMP_DB | SQLITE_OPEN_CREATE;
        config.debug = true;

        server = std::make_unique<RestServer>(addr, config);
        server->init(opts);
        server->serveThreaded();

        client = std::make_unique<Http::Client>();
        auto client_opts = Http::Client::options().threads(4);
        client->init(client_opts);
    }

    void TearDown() override {
        server->shutdown();
        client->shutdown();
    }
};

TEST_F(test_fixture_for_users, test_users) {
//  IN PARALLEL!!!!!
//    1. insert many users
//    2. select many users
//    3. update many users
//    4. delete many users

    const int timeout_sec = 5;
    const int num_iterations = 10;

    std::vector<Async::Promise<Http::Response>> responses;
    responses.reserve(num_iterations);
    const std::string url = "http://127.0.0.1:8888/0.0/users/create?";
    for (int i = 0; i < num_iterations; i++) {
        auto user = fake::user();
        auto post = url + "username=" + user.username + "&name=" + user.name;
        auto response = client->post(post)
                .send();
        response.then(
                [](Http::Response rsp) {
                    EXPECT_EQ(rsp.code(), Http::Code::Ok);
                },
                [](std::exception_ptr exceptionPtr) {
                    GTEST_FAIL();
                });
        responses.push_back(std::move(response));
    }

    auto sync = Async::whenAll(responses.begin(), responses.end());
    Async::Barrier<std::vector<Http::Response>> barrier(sync);
    barrier.wait_for(std::chrono::seconds(timeout_sec));
}


