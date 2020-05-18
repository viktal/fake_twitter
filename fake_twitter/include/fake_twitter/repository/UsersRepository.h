#pragma once

#include <sqlpp11/sqlpp11.h>
#include <optional>

#include "fake_twitter/model/User.h"
#include "fake_twitter/model/Followers.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"

#include "fake_twitter/sqlpp_models/FollowerTab.h"

#include "fake_twitter/repository/DBConnectionsPool.h"

#include <memory>
#include <utility>


namespace fake_twitter::repository {
    class UsersRepository {
    public:
        explicit UsersRepository(std::shared_ptr<DBConnectionsPool> pool) {
            this->pool = std::move(pool);
        }

        virtual ~UsersRepository() = default;

        virtual std::unique_ptr<model::User> get(PKey id);

        virtual model::User create(const std::string &name, const std::string &username);

        virtual bool drop(PKey id);

        virtual void update(PKey id,
                            std::optional<std::string> name,
                            std::optional<std::string> avatar);
        virtual std::unique_ptr<model::Followers> getfollow(PKey id);

        virtual bool follow(PKey author, PKey addresser);

        virtual bool unfollow(PKey author, PKey addresser);

    private:
        std::shared_ptr<DBConnectionsPool> pool;
        fake_twitter::sqlpp_models::TabUsers tabUsers;
        fake_twitter::sqlpp_models::TabFollower tabFollower;
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
                                                         (size_t) first.followers_count.value(),
                                                         (size_t) first.friends_count.value()});

        return user;
    }

    model::User UsersRepository::create(const std::string &name, const std::string &username) {
        auto newid = pool->run(insert_into(tabUsers).set(
                tabUsers.name = name,
                tabUsers.username = username,
                tabUsers.password_hash = 123,
                tabUsers.friends_count = 0,
                tabUsers.followers_count = 0,
                tabUsers.avatar = "path"));

        return std::move(model::User{PKey(newid), name, username, 123, "path", 0, 0});
    }

    bool UsersRepository::drop(PKey id) {
        return pool->run(remove_from(tabUsers).where(tabUsers.id == id));
    }

    void UsersRepository::update(PKey id, std::optional<std::string> name,
                                 std::optional<std::string> avatar) {
        if (!name && !avatar)
            return;

        // workaround to make dynamic update
        auto query = sqlpp::blank_update_t<sqlpp::sqlite3::connection>().single_table(tabUsers)
                .dynamic_set();
        if (name)
            query.assignments.add(tabUsers.name = name.value());
        if (avatar)
            query.assignments.add(tabUsers.avatar = avatar.value());

        pool->run(query.where(tabUsers.id == id));
    }

    std::unique_ptr<model::Followers> UsersRepository::getfollow(PKey id) {
        auto query = select(all_of(tabFollower)).from(tabFollower)
                .where(tabFollower.id == id);

        auto result = pool->run(query);
        if (result.empty()) {
            return nullptr;
        }

        auto &first = result.front();
        std::unique_ptr<model::Followers> follower;
        follower = std::make_unique<model::Followers>(model::Followers{first.id.value(),
                                                         first.author.value(),
                                                         first.addresser.value()});

        return follower;
    }

    bool UsersRepository::follow(PKey author, PKey addresser) {
        auto result = pool->run(select(all_of(tabFollower)).from(tabFollower)
                .where(tabFollower.author == author && tabFollower.addresser == addresser));
        if (result.empty()) {
            std::cout << "true" << std::endl;
            auto j = pool->run(insert_into(tabFollower).set(
                    tabFollower.author = author,
                    tabFollower.addresser = addresser));
            return true;
        }

        std::cout << "false" << std::endl;
        return false;
    }

    bool UsersRepository::unfollow(PKey author, PKey addresser) {
        return pool->run(remove_from(tabFollower).where(tabFollower.author == author && tabFollower.addresser == addresser));
    }

} //end namespace fake_twitter::repository
