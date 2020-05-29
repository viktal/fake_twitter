#pragma once

#include <gtest/gtest.h>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "fake_twitter/serializer/json.h"
#include "fake_twitter/utils.h"
#include "fake_twitter/model/User.h"

#include "httplib.h"

class EndpointTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto opts = Pistache::Http::Endpoint::options().threads(10).flags(
            Pistache::Tcp::Options::ReusePort |
            Pistache::Tcp::Options::ReuseAddr);
        endpoint = std::make_unique<Pistache::Http::Endpoint>(address);
        endpoint->init(opts);
        client = std::make_unique<httplib::Client>(address.host().c_str(),
                                                   Pistache::Port(8889));
    }

    void TearDown() override { endpoint->shutdown(); }

    void serveThreaded() {
        endpoint->setHandler(router.handler());
        endpoint->serveThreaded();
    }

    Pistache::Rest::Router router;
    Pistache::Address address =
        Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(8889));
    std::unique_ptr<httplib::Client> client;
    std::unique_ptr<Pistache::Http::Endpoint> endpoint;
};


httplib::Headers make_cookies(const fake_twitter::model::User& user) {
    auto json = fake_twitter::serialization::to_json(
        fake_twitter::utils::Session{user.id});
    auto headers = httplib::Headers();
    headers.insert({"Cookie", "session=" + json});
    return std::move(headers);
}
