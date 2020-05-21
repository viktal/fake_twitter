#pragma once

#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <optional>
#include <utility>

#include "fake_twitter/model/Followers.h"
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/repository/DBConnectionsPool.h"
#include "fake_twitter/sqlpp_models/FollowerTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "vector"

namespace fake_twitter::repository {
class NewsFeedRepository {
public:
    explicit NewsFeedRepository(std::shared_ptr<DBConnectionsPool> pool);

    virtual ~NewsFeedRepository() = default;

    virtual std::vector<model::Tweet> get(PKey id);
    virtual std::vector<model::Tweet> getUserBoard(PKey id);

private:
    std::shared_ptr<DBConnectionsPool> pool;
    fake_twitter::sqlpp_models::TabTweets tabTweets;
    fake_twitter::sqlpp_models::TabFollower tabFollower;
};
}  // namespace fake_twitter::repository
