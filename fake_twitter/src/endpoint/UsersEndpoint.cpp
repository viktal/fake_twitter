#include "fake_twitter/endpoint/UsersEndpoint.h"

using namespace fake_twitter;
using namespace fake_twitter::endpoints;

void UsersEndpoint::show(const Pistache::Rest::Request& request,
                         Pistache::Http::ResponseWriter response) {
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());
    std::unique_ptr<model::User> user = usersRepository->get(id);
    if (!user) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "No user with this id");
        return;
    } else {
        response.headers().add<Pistache::Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(*user));
    }
}

void UsersEndpoint::create(const Pistache::Rest::Request& request,
                           Pistache::Http::ResponseWriter response) {
    auto username_optional = request.query().get("username");
    auto name_optional = request.query().get("name");
    if (username_optional.isEmpty() || name_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto username = std::string(username_optional.get());
    auto name = std::string(name_optional.get());

    auto newUser = usersRepository->create(name, username);

    response.headers().add<Pistache::Http::Header::ContentType>(
        MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, serialization::to_json(newUser));
}

void UsersEndpoint::update(const Pistache::Rest::Request& request,
                           Pistache::Http::ResponseWriter response) {
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());

    auto name_optional = request.query().get("name");
    auto avatar_optional = request.query().get("avatar");

    std::optional<std::string> name;
    std::optional<std::string> avatar;

    if (!name_optional.isEmpty())
        name = std::optional<std::string>(name_optional.get());
    if (!avatar_optional.isEmpty())
        avatar = std::optional<std::string>(avatar_optional.get());

    usersRepository->update(id, name, avatar);
    response.send(Pistache::Http::Code::Ok);
}

void UsersEndpoint::drop(const Pistache::Rest::Request& request,
                         Pistache::Http::ResponseWriter response) {
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());
    if (usersRepository->drop(id))
        response.send(Pistache::Http::Code::Ok);
    else
        response.send(Pistache::Http::Code::Bad_Request);
}

void UsersEndpoint::showFollowTable(const Pistache::Rest::Request& request,
                                    Pistache::Http::ResponseWriter response) {
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto id = std::stol(id_optional.get());
    std::unique_ptr<model::Followers> followers =
        usersRepository->getfollow(id);
    if (!followers) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "No user with this id");
        return;
    } else {
        response.headers().add<Pistache::Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok,
                      serialization::to_json(*followers));
    }
}

void UsersEndpoint::follow(const Pistache::Rest::Request& request,
                           Pistache::Http::ResponseWriter response) {
    auto author_row = request.query().get("author");
    auto addresser_row = request.query().get("addresser");
    if (author_row.isEmpty() || addresser_row.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto author = std::stol(author_row.get());
    auto addresser = std::stol(addresser_row.get());

    if (author == addresser) response.send(Pistache::Http::Code::Bad_Request);

    if (usersRepository->follow(author, addresser))
        response.send(Pistache::Http::Code::Ok);
    else
        response.send(Pistache::Http::Code::Bad_Request);
}

void UsersEndpoint::unfollow(const Pistache::Rest::Request& request,
                             Pistache::Http::ResponseWriter response) {
    auto author_row = request.query().get("author");
    auto addresser_row = request.query().get("addresser");
    if (author_row.isEmpty() || addresser_row.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto author = std::stol(author_row.get());
    auto addresser = std::stol(addresser_row.get());

    if (usersRepository->unfollow(author, addresser))
        response.send(Pistache::Http::Code::Ok);
    else
        response.send(Pistache::Http::Code::Bad_Request);
}
