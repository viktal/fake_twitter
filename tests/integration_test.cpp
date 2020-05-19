#include <pistache/client.h>

#include <filesystem>

#include "fake_twitter/RestServer.h"
#include "fake_twitter/fake.h"
#include "gtest/gtest.h"

using namespace Pistache;
using namespace fake_twitter;
const std::string TMP_DB_NAME = "twitdb";
const std::string ADDRESS = "127.0.0.1";
const int PORT = 8888;

class test_fixture_restserver : public ::testing::Test {
public:
    std::unique_ptr<RestServer> server;
    std::unique_ptr<Http::Client> client;

    void SetUp() override {
        sqlpp::postgresql::connection_config config;
        config.host = "127.0.0.1";
        config.user = "twituser";
        config.password = "123";
        config.dbname = TMP_DB_NAME;
        config.debug = false;

        fake::postgresql_tables(
            std::make_shared<sqlpp::postgresql::connection_config>(config));

        Port port(PORT);
        Address addr(ADDRESS, port);

        auto opts = Http::Endpoint::options().threads(4).flags(
            Tcp::Options::ReusePort | Tcp::Options::ReuseAddr);

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
        if (std::filesystem::exists(TMP_DB_NAME)) std::remove(TMP_DB_NAME.c_str());
    }
};

void awaitall(std::vector<Async::Promise<Http::Response>>& awaitables) {
    auto sync = Async::whenAll(awaitables.begin(), awaitables.end());
    Async::Barrier<std::vector<Http::Response>> barrier(sync);
    barrier.wait();
}

void onfail(std::exception_ptr exceptionPtr) { GTEST_FAIL(); }

std::vector<model::User> make_users(Http::Client& client, int N) {
    std::vector<Async::Promise<Http::Response>> responses;
    std::vector<model::User> insertedUsers;
    std::mutex lock;

    // create random users
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/create?";
    for (int i = 0; i < N; i++) {
        auto userToInsert = fake::user::object();
        auto posturl = url + "username=" + userToInsert.username +
                       "&name=" + userToInsert.name;
        auto response = client.post(posturl).send();
        response.then(
            [userToInsert, &lock, &insertedUsers](Http::Response rsp) {
              EXPECT_EQ(rsp.code(), Http::Code::Ok);
              auto insertedUser = serialization::from_json<model::User>(rsp.body());
              EXPECT_EQ(insertedUser.name, userToInsert.name);
              EXPECT_EQ(insertedUser.username, userToInsert.username);

              std::lock_guard<std::mutex> guard(lock);
              insertedUsers.push_back(std::move(insertedUser));
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
    return insertedUsers;
}

void update_users(Http::Client& client, std::vector<model::User>& users) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/update?";

    for (auto& user : users) {
        auto puturl = url + "id=" + std::to_string(user.id);
        if (rand() % 2) {
            user.name = fake::user::name();
            puturl += "&name=" + user.name;
        }
        //        if (rand() % 2) {
        //            user.username = fake::user::username();
        //            puturl += "&username=" + user.username;
        //        }
        auto response = client.put(puturl).send();
        response.then(
            [&](Http::Response rsp) { EXPECT_EQ(rsp.code(), Http::Code::Ok); },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

void select_users(Http::Client& client, const std::vector<model::User>& users,
                  bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;

    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/show?";

    for (auto& user : users) {
        auto geturl = url + "id=" + std::to_string(user.id);
        auto response = client.get(geturl).send();
        response.then(
            [user, &expect_fail](Http::Response rsp) {
              if (expect_fail)
                  EXPECT_EQ(rsp.code(), Http::Code::Bad_Request);
              else {
                  EXPECT_EQ(rsp.code(), Http::Code::Ok);
                  auto dbUser = serialization::from_json<model::User>(rsp.body());
                  EXPECT_EQ(dbUser, user);
              }
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

void drop(Http::Client& client, const std::vector<model::User>& users,
          bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/drop?";

    for (auto& user : users) {
        auto delurl = url + "id=" + std::to_string(user.id);
        auto response = client.del(delurl).send();
        response.then(
            [&](Http::Response rsp) {
              if (expect_fail)
                  EXPECT_EQ(rsp.code(), Http::Code::Bad_Request);
              else
                  EXPECT_EQ(rsp.code(), Http::Code::Ok);
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

TEST_F(test_fixture_restserver, test_many_users_create_show) {
    const int N = 100;

    auto users = make_users(*client, N);
    update_users(*client, users);
    select_users(*client, users, false);
    drop(*client, users, false);

    // can't select after drop or drop twice
    select_users(*client, users, true);
    drop(*client, users, true);
}