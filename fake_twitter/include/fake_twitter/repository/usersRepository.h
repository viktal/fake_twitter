#pragma once

#include <sqlpp11/sqlpp11.h>
#include "fake_twitter/model/User.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"


#include <memory>


namespace fake_twitter::repository {
    class UsersRepository {
        std::unique_ptr<sqlpp::sqlite3::connection> db;

    public:
        explicit UsersRepository(std::unique_ptr<sqlpp::sqlite3::connection> db) {
            this->db = std::move(db);
        }
        virtual ~UsersRepository() = default;
        virtual std::unique_ptr<model::User> get(PKey id);
//        virtual std::unique_ptr<model::User> get2(PKey id) = 0;
    };

    std::unique_ptr<model::User> UsersRepository::get(PKey id) {
        fake_twitter::sqlpp_models::TabUsers tab;
        auto query = select(all_of(tab)).from(tab)
                .where(tab.id == id);

        auto result = (*db)(query);
        if (result.empty()) {
            return nullptr;
        }

        auto &first = result.front();
        std::unique_ptr<model::User> user;
        user = std::make_unique<model::User>(model::User{first.id.value(),
                                                         first.name.value(),
                                                         first.username.value(),
                                                         first.password_hash.value()});

        return user;
    }

} //end namespace fake_twitter::repository
