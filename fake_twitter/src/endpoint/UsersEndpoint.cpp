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
    auto password_optional = request.query().get("password");
    if (username_optional.isEmpty() || name_optional.isEmpty() ||
        password_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto username = std::string(username_optional.get());
    auto name = std::string(name_optional.get());
    auto password = std::string(password_optional.get());

    auto newUser = usersRepository->create(name, username, password);

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

    if (!request.cookies().has("session")) {
        response.send(Pistache::Http::Code::Unauthorized, "User unauthorized");
        return;
    }

    auto session = serialization::from_json<utils::Session>(
        request.cookies().get("session").value);

    if (id != session.user_id) {
        response.send(Pistache::Http::Code::Forbidden, "Not enough rights");
        return;
    }

    auto name_optional = request.query().get("name");

    std::optional<std::string> name;

    if (!name_optional.isEmpty())
        name = std::optional<std::string>(name_optional.get());

    usersRepository->update(id, name);
    response.send(Pistache::Http::Code::Ok, "User updated");
}

void UsersEndpoint::drop(const Pistache::Rest::Request& request,
                         Pistache::Http::ResponseWriter response) {
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());

    if (!request.cookies().has("session")) {
        response.send(Pistache::Http::Code::Unauthorized, "User unauthorized");
        return;
    }

    auto session = serialization::from_json<utils::Session>(
        request.cookies().get("session").value);

    if (id != session.user_id) {
        response.send(Pistache::Http::Code::Forbidden, "Not enough rights");
        return;
    }

    if (usersRepository->drop(id))
        response.send(Pistache::Http::Code::Ok, "User deleted");
    else
        response.send(Pistache::Http::Code::Bad_Request, "Cannot delete user");
}

void UsersEndpoint::showFollow(const Pistache::Rest::Request& request,
                               Pistache::Http::ResponseWriter response) {
    response.setMime(MIME(Text, Plain));

    if (!request.cookies().has("session")) {
        response.send(Pistache::Http::Code::Unauthorized,
                      "User in unauthorized");
        return;
    }
    auto id = serialization::from_json<utils::Session>(
            request.cookies().get("session").value)
            .user_id;
    std::vector<model::User_pr> followers = usersRepository->getfollow(id);

    response.setMime(MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, serialization::to_json(followers));
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

void UsersEndpoint::authorization(const Pistache::Rest::Request& request,
                                  Pistache::Http::ResponseWriter response) {
    auto username_optional = request.query().get("username");
    auto password_optional = request.query().get("password");
    if (username_optional.isEmpty() || password_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto username = std::string(username_optional.get());
    auto password = std::string(password_optional.get());

    auto user = usersRepository->get(username);

    if (!user) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Invalid username");
        return;
    }
    auto password_hash = utils::make_password_hash(user->salt, password);
    if (password_hash != user->password_hash) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Invalid password");
        return;
    }

    auto session = serialization::to_json(utils::Session{user->id});
    auto cookie = Pistache::Http::Cookie("session", session);
    cookie.path = Pistache::types::Some<std::string>("/");
    cookie.ext["SameSite"] = "Strict";
    response.cookies().add(cookie);

    response.setMime(MIME(Text, Plain));
    response.send(Pistache::Http::Code::Ok, "User is authorized");
}
