#pragma once

#include <sqlpp11/postgresql/postgresql.h>
#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <optional>
#include <utility>

#include "fake_twitter/model/Followers.h"
#include "fake_twitter/model/User.h"
#include "fake_twitter/repository/DBConnectionsPool.h"
#include "fake_twitter/sqlpp_models/FollowerTab.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/utils.h"

namespace fake_twitter::repository {
class UsersRepository {
public:
    explicit UsersRepository(std::shared_ptr<DBConnectionsPool> pool);

    virtual ~UsersRepository() = default;

    virtual std::unique_ptr<model::User> get(PKey id);

    virtual std::unique_ptr<model::User> get(const std::string& username);

    virtual std::unique_ptr<model::User> get(
        std::optional<PKey> id, std::optional<std::string> username);

    virtual model::User create(const std::string& name,
                               const std::string& username,
                               const std::string& password);

    virtual bool drop(PKey id);

    virtual void update(PKey id, std::optional<std::string> name);
    virtual std::unique_ptr<model::Followers> getfollow(PKey id);

    virtual bool follow(PKey author, PKey addresser);

    virtual bool unfollow(PKey author, PKey addresser);

private:
    std::shared_ptr<DBConnectionsPool> pool;
    fake_twitter::sqlpp_models::TabUsers tabUsers;
    fake_twitter::sqlpp_models::TabFollower tabFollower;
};

}  // end namespace fake_twitter::repository
