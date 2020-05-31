#include "fake_twitter/RestServer.h"

using namespace fake_twitter;
using namespace Pistache;
using namespace Pistache::Rest;

void RestServer::setupRoutes() {
    using namespace Pistache::Rest::Routes;

    Get(router, "/fakeshow", Routes::bind(&RestServer::fakeshow, this));

    Get(router, "/0.0/users/show",
        Routes::bind(&endpoints::UsersEndpoint::show, usersEndpoint));
    Post(router, "/0.0/users/create",
         Routes::bind(&endpoints::UsersEndpoint::create, usersEndpoint));
    Post(router, "/0.0/users/auth",
         Routes::bind(&endpoints::UsersEndpoint::authorization, usersEndpoint));
    Put(router, "/0.0/users/update",
        Routes::bind(&endpoints::UsersEndpoint::update, usersEndpoint));
    Delete(router, "/0.0/users/drop",
           Routes::bind(&endpoints::UsersEndpoint::drop, usersEndpoint));

    Get(router, "/0.0/newsfeed/show",
        Routes::bind(&endpoints::NewsFeedEndpoint::show, newsFeedEndpoint));

    Get(router, "/0.0/userboard/show",
        Routes::bind(&endpoints::NewsFeedEndpoint::showUserBoard,
                     newsFeedEndpoint));

    Post(router, "/0.0/users/follow",
         Routes::bind(&endpoints::UsersEndpoint::follow, usersEndpoint));
    Delete(router, "/0.0/users/unfollow",
           Routes::bind(&endpoints::UsersEndpoint::unfollow, usersEndpoint));
    Get(router, "/0.0/followers/show",
        Routes::bind(&endpoints::UsersEndpoint::showFollow, usersEndpoint));

    Get(router, "/0.0/comments/show.json",
        Routes::bind(&endpoints::CommentsEndpoint::show, commentsEndpoint));
    Post(router, "/0.0/CommentCreate/create",
         Routes::bind(&endpoints::CommentsEndpoint::create, commentsEndpoint));
    Put(router, "/0.0/CommentRaseLikes/update",
        Routes::bind(&endpoints::CommentsEndpoint::RaseLikes,
                     commentsEndpoint));
    Delete(
        router, "/0.0/commentDelete/delete",
        Routes::bind(&endpoints::CommentsEndpoint::Delete, commentsEndpoint));

    Get(router, "/0.0/tweets/show",
        Routes::bind(&endpoints::TweetsEndpoint::show, tweetsEndpoint));
    Post(router, "/0.0/tweets/create",
         Routes::bind(&endpoints::TweetsEndpoint::create, tweetsEndpoint));
    Delete(router, "/0.0/tweets/drop",
           Routes::bind(&endpoints::TweetsEndpoint::drop, tweetsEndpoint));

    Get(router, "/0.0/tags/show",
        Routes::bind(&endpoints::TagsEndpoint::show, tagsEndpoint));
    Post(router, "/0.0/tags/create",
         Routes::bind(&endpoints::TagsEndpoint::create, tagsEndpoint));

    Get(router, "/0.0/tagtweet/show",
        Routes::bind(&endpoints::TagTweetEndpoint::show, tagtweetEndpoint));
    Post(router, "/0.0/tagtweet/create",
         Routes::bind(&endpoints::TagTweetEndpoint::create, tagtweetEndpoint));

    Post(router, "/0.0/tweets/like",
         Routes::bind(&endpoints::TweetsEndpoint::like, tweetsEndpoint));
    Delete(router, "/0.0/tweets/unlike",
           Routes::bind(&endpoints::TweetsEndpoint::unlike, tweetsEndpoint));
}

RestServer::RestServer(Address addr,
                       const sqlpp::postgresql::connection_config& config) {
    httpEndpoint = std::make_shared<Http::Endpoint>(addr);

    auto config_ptr =
        std::make_shared<sqlpp::postgresql::connection_config>(config);
    auto connectionsPool = std::make_shared<repository::DBConnectionsPool>(
        std::move(config_ptr), 10);

    commentsRepository =
        std::make_unique<repository::CommentsRepository>(connectionsPool);
    commentsEndpoint =
        std::make_shared<endpoints::CommentsEndpoint>(commentsRepository);

    usersRepository =
        std::make_unique<repository::UsersRepository>(connectionsPool);
    usersEndpoint = std::make_shared<endpoints::UsersEndpoint>(usersRepository);

    tweetsRepository =
        std::make_unique<repository::TweetsRepository>(connectionsPool);
    tweetsEndpoint =
        std::make_shared<endpoints::TweetsEndpoint>(tweetsRepository);

    tagsRepository =
        std::make_unique<repository::TagsRepository>(connectionsPool);
    tagsEndpoint = std::make_shared<endpoints::TagsEndpoint>(tagsRepository);

    tagtweetRepository =
        std::make_unique<repository::TagTweetRepository>(connectionsPool);
    tagtweetEndpoint =
        std::make_shared<endpoints::TagTweetEndpoint>(tagtweetRepository);

    newsFeedRepository =
        std::make_unique<repository::NewsFeedRepository>(connectionsPool);
    newsFeedEndpoint =
        std::make_shared<endpoints::NewsFeedEndpoint>(newsFeedRepository);
}

void RestServer::init(const Http::Endpoint::Options& options) {
    httpEndpoint->init(options);
    setupRoutes();
    httpEndpoint->setHandler(router.handler());
}

void RestServer::serve() { httpEndpoint->serve(); }

void RestServer::serveThreaded() { httpEndpoint->serveThreaded(); }

void RestServer::shutdown() { httpEndpoint->shutdown(); }
void RestServer::fakeshow(const Request& request,
                          Pistache::Http::ResponseWriter response) {
    Http::serveFile(response, "fakeshow.html");
}
