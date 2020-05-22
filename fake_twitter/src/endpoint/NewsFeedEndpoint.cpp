#include "fake_twitter/endpoint/NewsFeedEndpoint.h"

using namespace fake_twitter;
using namespace fake_twitter::endpoints;

void NewsFeedEndpoint::show(const Pistache::Http::Request& request,
                            Pistache::Http::ResponseWriter response) {
    using fake_twitter::sqlpp_models::TabTweets;
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());
    std::vector<model::Tweet> tweetVector = newsFeedRepository->get(id);
    if (tweetVector.empty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "No followers with this id");
        return;
    }
    response.headers().add<Pistache::Http::Header::ContentType>(
        MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok,
                  serialization::to_json(tweetVector));
}
NewsFeedEndpoint::NewsFeedEndpoint(
    std::shared_ptr<repository::NewsFeedRepository> newsFeedRepository) {
    this->newsFeedRepository = std::move(newsFeedRepository);
};

void NewsFeedEndpoint::showUserBoard(const Pistache::Http::Request& request,
                                     Pistache::Http::ResponseWriter response) {
    using fake_twitter::sqlpp_models::TabTweets;
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());
    std::vector<model::Tweet> tweetVector =
        newsFeedRepository->getUserBoard(id);
    if (tweetVector.empty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "You have no tweets yet ");
        return;
    }
    response.headers().add<Pistache::Http::Header::ContentType>(
        MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok,
                  serialization::to_json(tweetVector));
}