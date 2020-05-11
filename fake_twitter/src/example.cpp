#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlite3.h>
#include <rapidjson/rapidjson.h>

#include "fake_twitter/model/User.h"
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/serializer/json.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"
#include "fake_twitter/endpoint/TweetsEndpoint.h"



using namespace Pistache;
using namespace rapidjson;
using namespace fake_twitter;
using fake_twitter::endpoints::UsersEndpoint;
using fake_twitter::endpoints::TweetsEndpoint;
namespace sql = sqlpp::sqlite3;

class RestServer {
public:
    RestServer(Address addr, sql::connection_config config) {
        httpEndpoint = std::make_shared<Http::Endpoint>(addr);
//        db = std::make_unique<sql::connection>(config);
        usersEndpoint = std::make_unique<UsersEndpoint>(std::make_unique<sql::connection>(config));
        tweetsEndpoint = std::make_unique<TweetsEndpoint>(std::make_unique<sql::connection>(config));
    }

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
                .threads(static_cast<int>(thr))
                .flags(Tcp::Options::ReusePort | Tcp::Options::ReuseAddr);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

    void setupRoutes() {
        using namespace Rest;

        Routes::Get(router, "/0.0/users/show.json", Routes::bind(&UsersEndpoint::show, usersEndpoint));
//        Routes::Get(router, "/0.0/users/show.json", Routes::bind(&StatsEndpoint::userShow, this));
        Routes::Post(router, "/0.0/users/create", Routes::bind(&UsersEndpoint::create, usersEndpoint));
        Routes::Get(router, "/0.0/tweets/show.json", Routes::bind(&TweetsEndpoint::show, tweetsEndpoint));
    }

private:
    std::shared_ptr<UsersEndpoint>  usersEndpoint;
    std::shared_ptr<TweetsEndpoint> tweetsEndpoint;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

//    std::unique_ptr<sql::connection> db;
};

int main(int argc, char *argv[]) {
    Port port(9080);
    int thr = 1;
    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));
        if (argc == 3)
            thr = std::stoi(argv[2]);
    }
    Address addr(Ipv4::any(), port);

//    cout << "Cores = " << hardware_concurrency() << endl;
//    cout << "Using " << thr << " threads" << endl;
    std::cout << "Start server" << std::endl;
    sql::connection_config config;
//    config.path_to_database = ":memory:";
    config.path_to_database = "/tmp/db.sqlite";
    config.flags = SQLITE_OPEN_READWRITE;
    config.debug = true;

    RestServer stats(addr, config);
    stats.init(thr);
    stats.start();
}
