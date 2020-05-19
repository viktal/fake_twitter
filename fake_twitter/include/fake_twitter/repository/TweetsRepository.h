#pragma once

#include <sqlpp11/sqlpp11.h>
#include <optional>
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/sqlpp_models/LikeTab.h"
#include "fake_twitter/repository/DBConnectionsPool.h"

#include <memory>
#include <utility>


namespace fake_twitter::repository {
    class TweetsRepository {
    public:
        explicit TweetsRepository(std::shared_ptr<DBConnectionsPool> pool) {
            this->pool = std::move(pool);
        }

        virtual ~TweetsRepository() = default;

        virtual std::unique_ptr<model::Tweet> get(PKey id);

        virtual model::Tweet create(const PKey &author, const std::string &body);

        virtual bool drop(PKey id);
//        virtual void update(PKey id,
//                            std::optional<std::string> name,
//                            std::optional<std::string> avatar);

        bool like(PKey author, PKey twit);

        bool unlike(PKey author, PKey twit);

    private:
        std::shared_ptr<DBConnectionsPool> pool;
        fake_twitter::sqlpp_models::TabTweets tabTweets;
        fake_twitter::sqlpp_models::TabLikes tabLikes;
    };

    std::unique_ptr<model::Tweet> TweetsRepository::get(PKey id) {
        auto query = select(all_of(tabTweets)).from(tabTweets)
                .where(tabTweets.id == id);

        auto result = pool->run(query);
        if (result.empty()) {
            return nullptr;
        }

        auto &first = result.front();
        std::unique_ptr<model::Tweet> tweet;
        tweet = std::make_unique<model::Tweet>(model::Tweet{first.id.value(),
                                                            first.body.value(),
                                                            first.author.value()});

        return tweet;
    }

    bool TweetsRepository::drop(PKey id) {
        return pool->run(remove_from(tabTweets).where(tabTweets.id == id));
    }

    model::Tweet TweetsRepository::create(const PKey &author, const std::string &body) {
        auto newid = pool->run(insert_into(tabTweets).set(
                tabTweets.author = author,
                tabTweets.body = body,
                tabTweets.create_date = std::chrono::system_clock::now(),
                tabTweets.retweets = 0,
                tabTweets.rating = 0));

        return std::move(model::Tweet{PKey(newid), body, PKey(author), "12.12.12", 0, 0});

    }

    bool TweetsRepository::like(PKey author, PKey twit) {
        auto result = pool->run(select(all_of(tabLikes)).from(tabLikes)
                                        .where(tabLikes.author == author &&
                                               tabLikes.twit == twit));
        if (result.empty()) {
            auto dummy2 = pool->run(sqlpp::update(tabTweets)
                                            .set(tabTweets.rating = tabTweets.rating + 1)
                                            .where(tabTweets.id == twit));
            if (!dummy2) {
                return false;
            }

            auto dummy = pool->run(insert_into(tabLikes).set(
                    tabLikes.author = author,
                    tabLikes.twit = twit));
            return true;
        }
        return false;
    }

    bool TweetsRepository::unlike(PKey author, PKey twit) {
        auto result = pool->run(
                remove_from(tabLikes).
                        where(tabLikes.author == author && tabLikes.twit == twit));
        if (!result) return false;
        auto dummy = pool->run(sqlpp::update(tabTweets)
                                       .set(tabTweets.rating = tabTweets.rating - 1)
                                       .where(tabTweets.id == twit));

        dummy = pool->run(sqlpp::update(tabTweets)
                                  .set(tabTweets.rating = tabTweets.rating - 1)
                                  .where(tabTweets.id == author));
        return true;
    }

} // end namespace fake_twitter::repository