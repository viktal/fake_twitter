#include "fake_twitter/endpoint/TweetsEndpoint.h"

using namespace fake_twitter::endpoints;

void TweetsEndpoint::show(const Pistache::Http::Request& request,
                          Pistache::Http::ResponseWriter response) {
    using fake_twitter::sqlpp_models::TabTweets;
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());

    std::unique_ptr<model::Tweet> tweet = tweetsRepository->get(id);
    if (!tweet) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "No tweet with this id");
        return;
    } else {
        response.headers().add<Pistache::Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(*tweet));
    }
}

void TweetsEndpoint::drop(const Pistache::Http::Request& request,
                          Pistache::Http::ResponseWriter response) {
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());

    std::unique_ptr<model::Tweet> tweet = tweetsRepository->get(id);

    if (!tweet.get()) {
        response.send(Pistache::Http::Code::Not_Found,
                      "Not tweet with this id");
        return;
    }

    if (!request.cookies().has("session")) {
        response.send(Pistache::Http::Code::Unauthorized, "User unauthorized");
        return;
    }

    auto session = serialization::from_json<utils::Session>(
        request.cookies().get("session").value);

    if (tweet->author != session.user_id) {
        response.send(Pistache::Http::Code::Forbidden, "Not enough rights");
        return;
    }

    if (tweetsRepository->drop(id))
        response.send(Pistache::Http::Code::Ok, "Tweet deleted");
    else
        response.send(Pistache::Http::Code::Bad_Request, "Cannot delete tweet");
}

void TweetsEndpoint::create(const Pistache::Http::Request& request,
                            Pistache::Http::ResponseWriter response) {
    auto author_optional = request.query().get("author");
    auto body_json = request.body();

    if (author_optional.isEmpty() || body_json.empty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }

    auto author = std::stol(author_optional.get());

    std::string body;

    rapidjson::Document document;
    document.Parse(body_json.c_str());

    if (document.HasParseError()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Cannot create empty tweet");
        return;
    }
    body = std::string(document["body"].GetString());

    if (!request.cookies().has("session")) {
        response.send(Pistache::Http::Code::Unauthorized,
                      "User is unauthorized");
        return;
    }

    auto session = serialization::from_json<utils::Session>(
        request.cookies().get("session").value);

    if (author != session.user_id) {
        response.send(Pistache::Http::Code::Forbidden, "Not enough rights");
        return;
    }

    auto newTwit = tweetsRepository->create(author, body);

    response.headers().add<Pistache::Http::Header::ContentType>(
        MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, serialization::to_json(newTwit));
}

void TweetsEndpoint::like(const Pistache::Http::Request& request,
                          Pistache::Http::ResponseWriter response) {
    auto author_row = request.query().get("author");
    auto twit_row = request.query().get("twit");
    if (author_row.isEmpty() || twit_row.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto author = std::stol(author_row.get());
    auto twit = std::stol(twit_row.get());

    if (tweetsRepository->like(author, twit))
        response.send(Pistache::Http::Code::Ok);
    else
        response.send(Pistache::Http::Code::Bad_Request);
}

void TweetsEndpoint::unlike(const Pistache::Http::Request& request,
                            Pistache::Http::ResponseWriter response) {
    auto author_row = request.query().get("author");
    auto twit_row = request.query().get("twit");
    if (author_row.isEmpty() || twit_row.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto author = std::stol(author_row.get());
    auto twit = std::stol(twit_row.get());

    if (tweetsRepository->unlike(author, twit))
        response.send(Pistache::Http::Code::Ok);
    else
        response.send(Pistache::Http::Code::Bad_Request);
}