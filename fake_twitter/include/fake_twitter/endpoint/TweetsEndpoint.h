#pragma once
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

#include <fake_twitter/serializer/json.h>
#include "fake_twitter/repository/TweetsRepository.h"

namespace fake_twitter::endpoints {
    class TweetsEndpoint {
    public:
        explicit TweetsEndpoint(std::shared_ptr<repository::TweetsRepository> tweetsRepository) {
            this->tweetsRepository = std::move(tweetsRepository);
        };
        void show (const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response);
        void drop (const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response);
        void create (const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response);
        void showAll(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response);
    private:
        std::shared_ptr<repository::TweetsRepository> tweetsRepository;
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
        std::unique_ptr<model::Tweet> tweet = tweetsRepository->get(id);
        if (!tweet) {
            response.send(Pistache::Http::Code::Bad_Request, "No tweet with this id");
            return;
        }
        else {
            response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Ok, serialization::to_json(*tweet));
        }
    }

    void TweetsEndpoint::drop(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());
        if (tweetsRepository->drop(id))
            response.send(Pistache::Http::Code::Ok);
        else
            response.send(Pistache::Http::Code::Bad_Request);
    }

    void TweetsEndpoint::create(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
        auto author_optional = request.query().get("author");
        auto body_json = request.body();

        if (author_optional.isEmpty() || body_json.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "Not found one or more parameters");
            return;
        }

        auto author = std::stol(author_optional.get());
        Document document;
        document.Parse(body_json.c_str());
        auto body = std::string(document["body"].GetString());

        auto newUser = tweetsRepository->create(author, body);

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(newUser));
    }
} //end namespace fake_twitter::endpoint
