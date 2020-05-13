#include <filesystem>
#include <pistache/client.h>
#include "gtest/gtest.h"
#include "fake_twitter/RestServer.h"
#include "fake_twitter/fake.h"

using namespace Pistache;
using namespace fake_twitter;
const std::string TMP_SQLITE_DB_PATH = "/tmp/tmp-tests-db.sqlite";
const std::string ADDRESS = "127.0.0.1";
const int PORT = 8888;


class test_fixture_restserver : public ::testing::Test {
public:
    std::unique_ptr<RestServer> server;
    std::unique_ptr<Http::Client> client;

    void SetUp() override {
        if (std::filesystem::exists(TMP_SQLITE_DB_PATH))
            std::remove(TMP_SQLITE_DB_PATH.c_str());

        fake::tables(TMP_SQLITE_DB_PATH);

        Port port(PORT);
        Address addr(ADDRESS, port);

        auto opts = Http::Endpoint::options()
                .threads(4)
                .flags(Tcp::Options::ReusePort | Tcp::Options::ReuseAddr);

        sql::connection_config config;
        config.path_to_database = TMP_SQLITE_DB_PATH;
        config.flags = SQLITE_OPEN_READWRITE;
        config.debug = false;

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
        if (std::filesystem::exists(TMP_SQLITE_DB_PATH))
            std::remove(TMP_SQLITE_DB_PATH.c_str());
    }
};

TEST_F(test_fixture_restserver, test_many_users_create_show) {
    const int num_iterations = 1000;
    const std::string url = "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/";
    const std::string url_create = url + "create/?";
    const std::string url_show = url + "show/?";


    std::vector<Async::Promise<Http::Response>> responses;
    std::vector<model::User> insertedUsers;
    std::mutex lock;

    auto faillmbda = [](std::exception_ptr exceptionPtr) {
        GTEST_FAIL();
    };

    // create random users
    for (int i = 0; i < num_iterations; i++) {
        auto userToInsert = fake::user();
        auto post = url_create + "username=" + userToInsert.username + "&name=" + userToInsert.name;
        auto response = client->post(post)
                .send();
        response.then(
                [&](Http::Response rsp) {
                    EXPECT_EQ(rsp.code(), Http::Code::Ok);
                    auto insertedUser = serialization::from_json<model::User>(rsp.body());
                    EXPECT_EQ(insertedUser.name, userToInsert.name);
                    EXPECT_EQ(insertedUser.username, userToInsert.username);

                    std::lock_guard<std::mutex> guard(lock);
                    insertedUsers.push_back(std::move(insertedUser));
                }, faillmbda
        );
        responses.push_back(std::move(response));
    }

    {
        auto sync = Async::whenAll(responses.begin(), responses.end());
        Async::Barrier<std::vector<Http::Response>> barrier(sync);
        barrier.wait();
    }

    // query and compare inserted users with on-server user
    responses.clear();
    for (auto &insertedUser: insertedUsers) {
        auto geturl = url_show + "id=" + std::to_string(insertedUser.id);
        auto response = client->get(geturl).send();
        response.then(
                [&](Http::Response rsp) {
                    EXPECT_EQ(rsp.code(), Http::Code::Ok);
                    auto onServerUser = serialization::from_json<model::User>(rsp.body());
                    EXPECT_EQ(insertedUser, onServerUser);
                }, faillmbda
        );
        responses.push_back(std::move(response));
    }

    {
        auto sync = Async::whenAll(responses.begin(), responses.end());
        Async::Barrier<std::vector<Http::Response>> barrier(sync);
        barrier.wait();
    }
}


