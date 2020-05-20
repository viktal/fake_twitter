#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <sqlpp11/postgresql/postgresql.h>

#include "fake_twitter/endpoint/TagsEndpoint.h"
#include "fake_twitter/endpoint/CommentsEndpoint.h"
#include "fake_twitter/endpoint/NewsFeedEndpoint.h"
#include "fake_twitter/endpoint/TweetsEndpoint.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"
#include "fake_twitter/repository/DBConnectionsPool.h"

namespace fake_twitter {

class RestServer {
public:
    RestServer(Pistache::Address addr,
               const sqlpp::postgresql::connection_config& config);
    void init(const Pistache::Http::Endpoint::Options& options);
    void serve();
    void serveThreaded();
    void shutdown();

private:
    void setupRoutes();
    std::shared_ptr<repository::UsersRepository> usersRepository;
    std::shared_ptr<repository::TweetsRepository> tweetsRepository;
    std::shared_ptr<repository::CommentsRepository> commentsRepository;
    std::shared_ptr<repository::NewsFeedRepository> newsFeedRepository;
    std::shared_ptr<repository::TagsRepository> tagsRepository;

    std::shared_ptr<endpoints::UsersEndpoint> usersEndpoint;
    std::shared_ptr<endpoints::TweetsEndpoint> tweetsEndpoint;
    std::shared_ptr<endpoints::CommentsEndpoint> commentsEndpoint;
    std::shared_ptr<endpoints::NewsFeedEndpoint> newsFeedEndpoint;
    std::shared_ptr<endpoints::TagsEndpoint> tagsEndpoint;

    std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
    Pistache::Rest::Router router;
};

}  // namespace fake_twitter
