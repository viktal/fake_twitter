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
#include "fake_twitter/repository/UsersRepository.h"
#include "fake_twitter/repository/DBConnectionsPool.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"
#include "fake_twitter/endpoint/TweetsEndpoint.h"

#include "fake_twitter/model/Comment.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"
#include "fake_twitter/endpoint/CommentsEndpoint.h"

using namespace Pistache;
using namespace rapidjson;
using namespace fake_twitter;
using fake_twitter::endpoints::UsersEndpoint;
using fake_twitter::endpoints::TweetsEndpoint;
using fake_twitter::endpoints::CommentsEndpoint;
namespace sql = sqlpp::sqlite3;

class RestServer {
public:
    RestServer(Address addr, sql::connection_config config) {
        httpEndpoint = std::make_shared<Http::Endpoint>(addr);
//        db = std::make_unique<sql::connection>(config);

        auto connection = std::make_unique<sqlpp::sqlite3::connection>(config);
        auto connectionsPool = std::make_shared<repository::DBConnectionsPool>(std::move(connection));

        commentsEndpoint = std::make_unique<CommentsEndpoint>(std::make_unique<sql::connection>(config));
        usersRepository = std::make_unique<repository::UsersRepository>(connectionsPool);
        usersEndpoint = std::make_shared<UsersEndpoint>(usersRepository);

//        tweetsEndpoint = std::make_unique<TweetsEndpoint>(std::make_unique<sql::connection>(config));
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

        Routes::Get(router, "/0.0/users/show", Routes::bind(&UsersEndpoint::show, usersEndpoint));
        Routes::Post(router, "/0.0/users/create", Routes::bind(&UsersEndpoint::create, usersEndpoint));
        Routes::Get(router, "/0.0/comments/show.json", Routes::bind(&CommentsEndpoint::show, commentsEndpoint));
        Routes::Get(router, "/0.0/commentsfortweet/show.json", Routes::bind(&CommentsEndpoint::showCommentsForTweet, commentsEndpoint));
        Routes::Delete(router, "/0.0/commentDelete/delete", Routes::bind(&CommentsEndpoint::Delete, commentsEndpoint));
        Routes::Put(router, "/0.0/CommentRaseLikes/update", Routes::bind(&CommentsEndpoint::RaseLikes, commentsEndpoint));
        Routes::Post(router, "/0.0/CommentCreate/create", Routes::bind(&CommentsEndpoint::create, commentsEndpoint));
//        Routes::Get(router, "/0.0/tweets/show.json", Routes::bind(&TweetsEndpoint::show, tweetsEndpoint));
    }
    std::shared_ptr<UsersEndpoint>  usersEndpoint;
    std::shared_ptr<TweetsEndpoint> tweetsEndpoint;
    std::shared_ptr<CommentsEndpoint> commentsEndpoint;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    std::shared_ptr<repository::UsersRepository> usersRepository;
    Rest::Router router;

//    std::unique_ptr<sql::connection> db;
};