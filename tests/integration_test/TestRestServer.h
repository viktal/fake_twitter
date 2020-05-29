#pragma once

#include "fake_twitter/RestServer.h"
#include "fake_twitter/fake.h"
#include "gtest/gtest.h"

const std::string TMP_DB_NAME = "twitdb";
using namespace Pistache;
using namespace fake_twitter;

const std::string ADDRESS = "127.0.0.1";
const int PORT = 8888;

class test_fixture_restserver : public ::testing::Test {
public:
    std::unique_ptr<RestServer> server;
    std::unique_ptr<Http::Client> client;
    sqlpp::postgresql::connection_config config;

    void SetUp() override {
        config.host = "127.0.0.1";
        config.user = "twituser";
        config.password = "123";
        config.dbname = TMP_DB_NAME;
        config.debug = false;

        fake::drop_postgresql_tables(
            std::make_shared<sqlpp::postgresql::connection_config>(config));

        fake::create_postgresql_tables(
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
        fake::drop_postgresql_tables(
            std::make_shared<sqlpp::postgresql::connection_config>(config));
        server->shutdown();
        client->shutdown();
        if (std::filesystem::exists(TMP_DB_NAME))
            std::remove(TMP_DB_NAME.c_str());
    }
};

void awaitall(std::vector<Async::Promise<Http::Response>>& awaitables) {
    auto sync = Async::whenAll(awaitables.begin(), awaitables.end());
    Async::Barrier<std::vector<Http::Response>> barrier(sync);
    barrier.wait();
}

void onfail(std::exception_ptr exceptionPtr) { GTEST_FAIL(); }
