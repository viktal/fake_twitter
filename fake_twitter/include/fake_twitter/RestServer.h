#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <rapidjson/rapidjson.h>

#include "fake_twitter/model/User.h"
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/serializer/json.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/repository/usersRepository.h"
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

        usersRepository = std::make_unique<repository::UsersRepository>
                (std::make_unique<sqlpp::sqlite3::connection>(config));
        usersEndpoint = std::make_shared<UsersEndpoint>(usersRepository);

        tweetsEndpoint = std::make_unique<TweetsEndpoint>(std::make_unique<sql::connection>(config));
    }

    void init(Http::Endpoint::Options options) {
        httpEndpoint->init(options);
        setupRoutes();
        httpEndpoint->setHandler(router.handler());
    }

    void serve() {
        httpEndpoint->serve();
    }

    void serveThreaded() {
        httpEndpoint->serveThreaded();
    }

    void shutdown() {
        httpEndpoint->shutdown();
    }

private:
    void setupRoutes() {
        using namespace Rest;

        Routes::Get(router, "/0.0/users/show.json", Routes::bind(&UsersEndpoint::show, usersEndpoint));
        Routes::Post(router, "/0.0/users/create", Routes::bind(&UsersEndpoint::create, usersEndpoint));
        Routes::Get(router, "/0.0/tweets/show.json", Routes::bind(&TweetsEndpoint::show, tweetsEndpoint));
    }
    std::shared_ptr<UsersEndpoint>  usersEndpoint;
    std::shared_ptr<TweetsEndpoint> tweetsEndpoint;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    std::shared_ptr<repository::UsersRepository> usersRepository;
    Rest::Router router;

//    std::unique_ptr<sql::connection> db;
};