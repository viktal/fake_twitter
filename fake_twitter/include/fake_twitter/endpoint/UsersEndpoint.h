#pragma once
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

namespace fake_twitter::endpoints {
    class UsersEndpoint {
    public:
        explicit UsersEndpoint(std::unique_ptr<sqlpp::sqlite3::connection> db) {
            this->db = std::move(db);
        };
        void show(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
        void create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    private:
        std::unique_ptr<sqlpp::sqlite3::connection> db;
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

        TabUsers tab;
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No user with this id");
            return;
        }

        auto& first = result.front();
        model::User user = {first.id.value(), first.name.value(), first.username.value(), first.password_hash.value()};

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(user));
    }

    void UsersEndpoint::create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabUsers;
        auto username_optional = request.query().get("username");
        auto name_optional = request.query().get("name");
        auto password_optional = request.query().get("password_hash");
        if (username_optional.isEmpty() ||
            name_optional.isEmpty() ||
            password_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "Not found one or more parameters");
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
            response.send(Pistache::Http::Code::Bad_Request, "No user with this username");
            return;
        }

        auto& first = result.front();
        model::User user = {first.id.value(), first.name.value(), first.username.value(), first.password_hash.value()};

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(user));
    }
}
