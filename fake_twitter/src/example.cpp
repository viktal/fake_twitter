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


using namespace Pistache;
using namespace rapidjson;
using namespace fake_twitter;
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
        Routes::Post(router, "/0.0/users/create", Routes::bind(&StatsEndpoint::userCreate, this));
        Routes::Get(router, "/0.0/tweets/show.json", Routes::bind(&StatsEndpoint::tweetShow, this));
    }

private:
    void userShow(const Rest::Request &request, Http::ResponseWriter response) {
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

    void userCreate(const Rest::Request &request, Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabUsers;
        auto username_optional = request.query().get("username");
        auto name_optional = request.query().get("name");
        auto password_optional = request.query().get("password_hash");
        if (username_optional.isEmpty() ||
            name_optional.isEmpty() ||
            password_optional.isEmpty()) {
            response.send(Http::Code::Bad_Request, "Not found one or more parameters");
            return;
        }
        auto username = std::string(username_optional.get());
        auto name = std::string(name_optional.get());
        auto password = std::stol(password_optional.get());


        TabUsers tabUsers;
        (*db)(insert_into(tabUsers).set(
                tabUsers.name = name,
                tabUsers.username = username,
                tabUsers.password_hash = password,
                tabUsers.friends_count = 0,
                tabUsers.followers_count = 0,
                tabUsers.avatar = "path"));

        for (const auto &row : (*db)(select(all_of(tabUsers)).from(tabUsers).unconditionally())) {
            std::cout << row.id << " " <<
                      row.name << " " <<
                      row.username << " " <<
                      row.avatar << " " <<
                      row.followers_count << " " <<
                      row.friends_count << " " <<
                      row.password_hash << " " << std::endl;
        };

        auto result = (*db)(select(all_of(tabUsers)).from(tabUsers)
                                    .where(tabUsers.username == username));
        if (result.empty()) {
            response.send(Http::Code::Bad_Request, "No user with this username");
            return;
        }

        auto& first = result.front();
        model::User user = {first.id.value(), first.name.value(), first.username.value(), first.password_hash.value()};

        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Ok, serialization::to_json(user));

    }

    void tweetShow(const Rest::Request &request, Http::ResponseWriter response) {
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

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

    std::unique_ptr<sql::connection> db;

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

    StatsEndpoint stats(addr, config);
    stats.init(thr);
    stats.start();
}
