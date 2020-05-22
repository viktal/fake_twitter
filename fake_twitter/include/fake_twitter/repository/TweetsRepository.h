#pragma once

#include <sqlpp11/postgresql/postgresql.h>
#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <optional>
#include <utility>

#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/repository/DBConnectionsPool.h"
#include "fake_twitter/sqlpp_models/LikeTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"

namespace fake_twitter::repository {
class TweetsRepository {
public:
    explicit TweetsRepository(std::shared_ptr<DBConnectionsPool> pool);

    virtual ~TweetsRepository() = default;

    virtual std::unique_ptr<model::Tweet> get(PKey id);
    virtual model::Tweet create(const PKey& author, const std::string& body);
    virtual bool drop(PKey id);

    virtual bool like(PKey author, PKey twit);
    virtual bool unlike(PKey author, PKey twit);

private:
    std::shared_ptr<DBConnectionsPool> pool;
    fake_twitter::sqlpp_models::TabTweets tabTweets;
    fake_twitter::sqlpp_models::TabLikes tabLikes;
};

}  // end namespace fake_twitter::repository