#include "fake_twitter/repository/TweetsRepository.h"

using namespace fake_twitter;
using fake_twitter::repository::TweetsRepository;

TweetsRepository::TweetsRepository(std::shared_ptr<DBConnectionsPool> pool) {
    this->pool = std::move(pool);
}

std::unique_ptr<model::Tweet> TweetsRepository::get(PKey id) {
    auto query =
        select(all_of(tabTweets)).from(tabTweets).where(tabTweets.id == id);

    auto result = pool->run(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::Tweet> tweet;
    tweet = std::make_unique<model::Tweet>(model::Tweet{
        first.id.value(), first.body.value(), first.author.value()});

    return tweet;
}

bool TweetsRepository::drop(PKey id) {
    return pool->run(remove_from(tabTweets).where(tabTweets.id == id));
}

model::Tweet TweetsRepository::create(const PKey& author,
                                      const std::string& body) {
    auto newid = pool->run(sqlpp::postgresql::insert_into(tabTweets).set(
        tabTweets.author = author, tabTweets.body = body,
        tabTweets.create_date = std::chrono::system_clock::now(),
        tabTweets.retweets = 0, tabTweets.rating = 0).returning(tabTweets.id));

    return std::move(
        model::Tweet{PKey(newid.front().id.value()), body, PKey(author), "12.12.12", 0, 0});
}

bool TweetsRepository::like(PKey author, PKey twit) {
    auto result = pool->run(
        select(all_of(tabLikes))
            .from(tabLikes)
            .where(tabLikes.author == author && tabLikes.twit == twit));
    if (result.empty()) {
        auto dummy2 =
            pool->run(sqlpp::update(tabTweets)
                          .set(tabTweets.rating = tabTweets.rating + 1)
                          .where(tabTweets.id == twit));
        if (!dummy2) {
            return false;
        }

        auto dummy = pool->run(insert_into(tabLikes).set(
            tabLikes.author = author, tabLikes.twit = twit));
        return true;
    }
    return false;
}

bool TweetsRepository::unlike(PKey author, PKey twit) {
    auto result = pool->run(remove_from(tabLikes).where(
        tabLikes.author == author && tabLikes.twit == twit));
    if (!result) return false;
    auto dummy = pool->run(sqlpp::update(tabTweets)
                               .set(tabTweets.rating = tabTweets.rating - 1)
                               .where(tabTweets.id == twit));

    dummy = pool->run(sqlpp::update(tabTweets)
                          .set(tabTweets.rating = tabTweets.rating - 1)
                          .where(tabTweets.id == author));
    return true;
}
