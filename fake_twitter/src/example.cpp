#include <algorithm>
#include <iostream>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "fake_twitter/model/User.h"
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/model/Comment.h"

#include <rapidjson/rapidjson.h>
#include "fake_twitter/serializer/json.h"

#include <sqlpp11/custom_query.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlite3.h>

#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"


using namespace Pistache;
namespace sql = sqlpp::sqlite3;

class StatsEndpoint {
public:
    explicit StatsEndpoint(Address addr, sql::connection_config config) {
        httpEndpoint = std::make_shared<Http::Endpoint>(addr);
        db = std::make_unique<sql::connection>(config);
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
        Routes::Get(router, "/0.0/users/show.json", Routes::bind(&StatsEndpoint::userShow, this));
        Routes::Get(router, "/0.0/tweets/show.json", Routes::bind(&StatsEndpoint::tweetShow, this));
        Routes::Get(router, "/0.0/comments/show.json", Routes::bind(&StatsEndpoint::commentShow, this));
        Routes::Get(router, "/0.0/commentsfortweet/show.json", Routes::bind(&StatsEndpoint::CommentForTweetShow, this));
        Routes::Delete(router, "/0.0/commentDelete/delete", Routes::bind(&StatsEndpoint::CommentDelete, this));
    }

private:
    void CommentDelete(const Rest::Request &request, Http::ResponseWriter response) {
        std::cout<<"sosi" << std::endl;
        using namespace rapidjson;
        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = stol(id_optional.get());
         TabComments tab;
        (*db)(remove_from(tab).where(tab.id == id));

            response.send(Pistache::Http::Code::Ok, "Comment deleted");
    }

    void userShow(const Rest::Request &request, Http::ResponseWriter response) {
        using namespace rapidjson;

        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabUsers;

        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());

        TabUsers tab;
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        if (result.empty()) {
            response.send(Http::Code::Bad_Request, "No user with this id");
            return;
        }

        auto& first = result.front();
        model::User user = {first.id.value(), first.name.value(), first.username.value(), first.password_hash.value()};

        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Ok, serialization::to_json(user));

    }

    void tweetShow(const Rest::Request &request, Http::ResponseWriter response) {
        using namespace rapidjson;

        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabTweets;

        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());

        TabTweets tab;
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        if (result.empty()) {
            response.send(Http::Code::Bad_Request, "No tweet with this id");
            return;
        }

        auto& first = result.front();
        model::Tweet tweet = {first.id.value(), first.body.value(), first.author.value()};

        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Ok, serialization::to_json(tweet));

    }

    void commentShow(const Rest::Request &request, Http::ResponseWriter response) {
        using namespace rapidjson;

        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;

        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());

        TabComments tab;
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        if (result.empty()) {
            response.send(Http::Code::Bad_Request, "No comment with this id");
            return;
        }

        auto& first = result.front();
        model::Comment comment = {first.id.value(), first.body.value(), first.author.value(), first.comment_for.value()};

        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Ok, serialization::to_json(comment));

    }

    void CommentForTweetShow(const Rest::Request &request, Http::ResponseWriter response) {
        using namespace rapidjson;

        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;

        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());
        TabComments tab;
        //auto result = (*db)(select(all_of(tab)).from(tab)
                                    //.where(tab.comment_for == id));
        model::Comment* comment = new model::Comment[6];
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.comment_for == id));
        if (result.empty()) {
            response.send(Http::Code::Bad_Request, "No comment with this id");
            delete []comment;
            return;
        }
        int i;
        while (!result.empty()) {
            const auto &row = result.front();
            comment[i].set(row.id.value(), row.body.value(), row.author.value(), row.comment_for.value());
            i++;
            result.pop_front();
        }
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Ok, serialization::to_json(comment, i));
        delete []comment;
        return;
    }
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

    std::unique_ptr<sql::connection> db;

};

int main(int argc, char *argv[]) {
    Port port(9080);

    int thr = 2;

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

    StatsEndpoint stats(addr, config);
    stats.init(thr);
    stats.start();
}
