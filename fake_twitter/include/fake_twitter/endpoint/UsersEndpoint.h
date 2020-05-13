#pragma once
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>
#include <fake_twitter/serializer/json.h>

#include "fake_twitter/repository/UsersRepository.h"

namespace fake_twitter::endpoints {
    class UsersEndpoint {
    public:
        explicit UsersEndpoint(std::shared_ptr<repository::UsersRepository> usersRepository) {
            this->usersRepository = std::move(usersRepository);
        };

        void show(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
        void create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    private:
        std::shared_ptr<repository::UsersRepository> usersRepository;
    };

    void UsersEndpoint::show(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabUsers;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());


        std::unique_ptr<model::User> user = usersRepository->get(id);
        if (!user) {
            response.send(Pistache::Http::Code::Bad_Request, "No user with this id");
            return;
        }
        else {
            response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Ok, serialization::to_json(*user));
        }

    }

    void UsersEndpoint::create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabUsers;
        auto username_optional = request.query().get("username");
        auto name_optional = request.query().get("name");
        if (username_optional.isEmpty() ||
            name_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "Not found one or more parameters");
            return;
        }
        auto username = std::string(username_optional.get());
        auto name = std::string(name_optional.get());

        auto newUser = usersRepository->create(name, username);

        response.headers().template add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(newUser));
    }
}
