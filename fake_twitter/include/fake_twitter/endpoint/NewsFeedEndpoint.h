#pragma once
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

#include <fake_twitter/serializer/json.h>
#include "fake_twitter/repository/NewsFeedRepository.h"

namespace fake_twitter::endpoints {
    class NewsFeedEndpoint {
    public:
        explicit NewsFeedEndpoint(std::shared_ptr<repository::NewsFeedRepository>
        newsFeedRepository) {
            this->newsFeedRepository = std::move(newsFeedRepository);
        };

        void show(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response);

    private:
        std::shared_ptr<repository::NewsFeedRepository> newsFeedRepository;
    };

    void NewsFeedEndpoint::show(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabTweets;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());
        std::unique_ptr<model::Tweet> tweet = newsFeedRepository->get(id);
        if (!tweet) {
            response.send(Pistache::Http::Code::Bad_Request, "No followers with this id");
            return;
        } else {
            response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Ok, serialization::to_json(*tweet));
        }
    }
}