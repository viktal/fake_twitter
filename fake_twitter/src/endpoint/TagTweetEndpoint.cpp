#include "fake_twitter/endpoint/TagTweetEndpoint.h"

using namespace fake_twitter::endpoints;

void TagTweetEndpoint::show(const Pistache::Http::Request& request,
                          Pistache::Http::ResponseWriter response) {
    using fake_twitter::sqlpp_models::TabTagTweet;
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());
    std::unique_ptr<model::TagTweet> tagtweet = tagtweetRepository->get(id);
    if (!tagtweet) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "No tag with this id");
        return;
    } else {
        response.headers().add<Pistache::Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(*tagtweet));
    }
}

void TagTweetEndpoint::create(const Pistache::Http::Request& request,
                            Pistache::Http::ResponseWriter response) {
    auto tweetID_optional = request.query().get("tweetID");
    auto tagID_optional = request.query().get("tagID");
    if (tweetID_optional.isEmpty() || tagID_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters erik aaaaaaaaaaaaa");
        return;
    }
    auto tweetID = std::stol(tweetID_optional.get());
    auto tagID = std::stol(tagID_optional.get());

    auto newTagTweet = tagtweetRepository->create(tweetID, tagID);

    response.headers().add<Pistache::Http::Header::ContentType>(
        MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, serialization::to_json(newTagTweet));
}