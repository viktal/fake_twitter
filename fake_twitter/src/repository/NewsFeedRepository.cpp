#include "fake_twitter/repository/NewsFeedRepository.h"

using namespace fake_twitter;
using namespace fake_twitter::repository;

std::vector<model::Tweet> NewsFeedRepository::get(PKey id) {
    auto queryFollower = select(all_of(tabFollower))
                             .from(tabFollower)
                             .where(tabFollower.author == id);
    std::vector<model::Tweet> tweet_vector;
    auto resultFollower = pool->run(queryFollower);
    if (resultFollower.empty()) {
        return tweet_vector;
    }
    while (!resultFollower.empty()) {
        auto& firstFollower = resultFollower.front();
        auto queryTweet =
            select(all_of(tabTweets))
                .from(tabTweets)
                .where(tabTweets.author == firstFollower.addresser.value());

        auto resultTweet = pool->run(queryTweet);
        if (resultTweet.empty()) {
            return tweet_vector;
        }
        while (!resultTweet.empty()) {
            auto& firstTweet = resultTweet.front();
            model::Tweet t = {firstTweet.id.value(), firstTweet.body.value(),
                              firstTweet.author.value()};
            tweet_vector.push_back(t);
            resultTweet.pop_front();
        }
        resultFollower.pop_front();
    }
    return tweet_vector;
}
NewsFeedRepository::NewsFeedRepository(
    std::shared_ptr<DBConnectionsPool> pool) {
    this->pool = std::move(pool);
}
