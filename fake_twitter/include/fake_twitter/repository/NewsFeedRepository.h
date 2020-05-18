#pragma once

#include <sqlpp11/sqlpp11.h>
#include <optional>
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/model/Followers.h"
#include "fake_twitter/sqlpp_models/FollowerTab.h"
#include "fake_twitter/repository/DBConnectionsPool.h"

#include <memory>
#include <utility>


namespace fake_twitter::repository {
    class NewsFeedRepository {
    public:
        explicit NewsFeedRepository(std::shared_ptr<DBConnectionsPool> pool) {
            this->pool = std::move(pool);
        }

        virtual ~NewsFeedRepository() = default;

        virtual std::unique_ptr<model::Tweet> get(PKey id);

    private:
        std::shared_ptr<DBConnectionsPool> pool;
        fake_twitter::sqlpp_models::TabTweets tabTweets;
        fake_twitter::sqlpp_models::TabFollower tabFollower;
    };

    std::unique_ptr<model::Tweet> NewsFeedRepository::get(PKey id) {
        auto queryFollower = select(all_of(tabFollower)).from(tabFollower)
                .where(tabFollower.author == id);
        auto resultFollower = pool->run(queryFollower);
        if (resultFollower.empty()) {
            return nullptr;
        }
        auto &firstFollower = resultFollower.front();
        auto queryTweet = select(all_of(tabTweets)).from(tabTweets)
                .where(tabTweets.author == firstFollower.addresser.value());

        auto resultTweet = pool->run(queryTweet);
        if (resultTweet.empty()) {
            return nullptr;
        }

        auto &firstTweet = resultTweet.front();
        std::unique_ptr<model::Tweet> tweet;
        tweet = std::make_unique<model::Tweet>(model::Tweet{firstTweet.id.value(),
                                                            firstTweet.body.value(),
                                                            firstTweet.author.value()});

        return tweet;
    }
}

