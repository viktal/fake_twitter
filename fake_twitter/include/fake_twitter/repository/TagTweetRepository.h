#pragma once

#include <sqlpp11/postgresql/postgresql.h>
#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <optional>
#include <utility>

#include "fake_twitter/model/TagTweet.h"
#include "fake_twitter/repository/DBConnectionsPool.h"
#include "fake_twitter/sqlpp_models/TagTweetTab.h"

namespace fake_twitter::repository {
class TagTweetRepository {
public:
    explicit TagTweetRepository(std::shared_ptr<DBConnectionsPool> pool);

    virtual ~TagTweetRepository() = default;

    virtual std::unique_ptr<model::TagTweet> get(PKey id);
    virtual model::TagTweet create(const PKey& Tweet, const PKey& Tag);

private:
    std::shared_ptr<DBConnectionsPool> pool;
    fake_twitter::sqlpp_models::TabTagTweet tabTagTweet;
};

}  // end namespace fake_twitter::repository