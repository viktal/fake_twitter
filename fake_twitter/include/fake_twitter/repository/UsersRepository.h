#pragma once

#include <sqlpp11/sqlpp11.h>
#include "fake_twitter/model/User.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/repository/DBConnectionsPool.h"

#include <memory>
#include <utility>


namespace fake_twitter::repository {
    class UsersRepository {
    public:
        explicit UsersRepository(std::shared_ptr<DBConnectionsPool> pool)
        {
            this->pool = std::move(pool);
        }
        virtual ~UsersRepository() = default;
        virtual std::unique_ptr<model::User> get(PKey id);
        virtual model::User create(const std::string& name, const std::string& username);
    private:
        std::shared_ptr<DBConnectionsPool> pool;
        fake_twitter::sqlpp_models::TabUsers tabUsers;
    };

    std::unique_ptr<model::User> UsersRepository::get(PKey id) {
        auto query = select(all_of(tabUsers)).from(tabUsers)
                .where(tabUsers.id == id);

        auto result = pool->run(query);
        if (result.empty()) {
            return nullptr;
        }

        auto &first = result.front();
        std::unique_ptr<model::User> user;
        user = std::make_unique<model::User>(model::User{first.id.value(),
                                                         first.name.value(),
                                                         first.username.value(),
                                                         first.password_hash.value(),
                                                         first.avatar.value(),
                                                         (size_t)first.followers_count.value(),
                                                         (size_t)first.friends_count.value()});

        return user;
    }

    model::User UsersRepository::create(const std::string& name, const std::string& username) {
        auto newid = pool->run(insert_into(tabUsers).set(
                tabUsers.name = name,
                tabUsers.username = username,
                tabUsers.password_hash = 123,
                tabUsers.friends_count = 0,
                tabUsers.followers_count = 0,
                tabUsers.avatar = "path"));

        return std::move(model::User{PKey(newid), name, username, 123, "path", 0, 0});
    }

} //end namespace fake_twitter::repository
