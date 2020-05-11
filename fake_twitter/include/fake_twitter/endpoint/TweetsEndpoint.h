#pragma once
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

namespace fake_twitter::endpoints {
    class TweetsEndpoint {
    public:
        explicit TweetsEndpoint(std::unique_ptr<sqlpp::sqlite3::connection> db) {
            this->db = std::move(db);
        }
        void show (const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response);
//        void drop (const Pistache::)
    private:
        std::unique_ptr<sqlpp::sqlite3::connection> db;
    };

    void TweetsEndpoint::show(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabTweets;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());
        TabTweets tab;
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No tweet with this id");
            return;
        }

        auto& first = result.front();
        model::Tweet tweet = {first.id.value(), first.body.value(), first.author.value()};

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(tweet));
    }
} //end namespace fake_twitter::endpoint
