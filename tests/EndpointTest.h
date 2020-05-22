#pragma once

#include <gtest/gtest.h>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

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
