#include "fake_twitter/repository/TweetsRepository.h"

using namespace fake_twitter;
using fake_twitter::repository::TweetsRepository;

TweetsRepository::TweetsRepository(std::shared_ptr<DBConnectionsPool> pool) {
    this->pool = std::move(pool);
}

std::unique_ptr<model::Tweet> TweetsRepository::get(PKey id) {
    auto query =
        select(all_of(tabTweets)).from(tabTweets).where(tabTweets.id == id);

    auto result = pool->get_connection()(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::Tweet> tweet;
    tweet = std::make_unique<model::Tweet>(
        model::Tweet{first.id.value(), first.body.value(), first.author.value(),
                     std::chrono::time_point_cast<std::chrono::seconds>(
                         first.create_date.value()),
                     first.rating.value(), first.retweets.value()});

    return tweet;
}

bool TweetsRepository::drop(PKey id) {
    return pool->get_connection()(
        remove_from(tabTweets).where(tabTweets.id == id));
}

model::Tweet TweetsRepository::create(const PKey& author,
                                      const std::string& body) {
    auto time = std::chrono::time_point_cast<std::chrono::seconds>(
        std::chrono::system_clock::now());
    auto newid = pool->get_connection()(
        sqlpp::postgresql::insert_into(tabTweets)
            .set(tabTweets.author = author, tabTweets.body = body,
                 tabTweets.create_date = time, tabTweets.retweets = 0,
                 tabTweets.rating = 0)
            .returning(tabTweets.id));

    return std::move(model::Tweet{PKey(newid.front().id.value()), body,
                                  PKey(author), time, 0, 0});
}

bool TweetsRepository::like(PKey author, PKey twit) {
    auto result = pool->get_connection()(
        select(all_of(tabLikes))
            .from(tabLikes)
            .where(tabLikes.author == author && tabLikes.twit == twit));
    if (result.empty()) {
        auto dummy2 = pool->get_connection()(
            sqlpp::update(tabTweets)
                .set(tabTweets.rating = tabTweets.rating + 1)
                .where(tabTweets.id == twit));
        if (!dummy2) {
            return false;
        }

        auto dummy = pool->get_connection()(insert_into(tabLikes).set(
            tabLikes.author = author, tabLikes.twit = twit));
        return true;
    }
    return false;
}

bool TweetsRepository::unlike(PKey author, PKey twit) {
    auto result = pool->get_connection()(remove_from(tabLikes).where(
        tabLikes.author == author && tabLikes.twit == twit));
    if (!result) return false;
    auto dummy =
        pool->get_connection()(sqlpp::update(tabTweets)
                                   .set(tabTweets.rating = tabTweets.rating - 1)
                                   .where(tabTweets.id == twit));

    dummy =
        pool->get_connection()(sqlpp::update(tabTweets)
                                   .set(tabTweets.rating = tabTweets.rating - 1)
                                   .where(tabTweets.id == author));
    return true;
}

void TweetsRepository::parse(const PKey& tweetID, const std::string& body) {
    std::vector<std::string> tagvec;
    std::string tag;
    std::stringstream ss(body);
    while (ss >> tag) {
        if (tag[0] == '#') {
            tagvec.push_back(tag.erase(0, 1));
        }
    }
    for (std::vector<std::string>::const_iterator i = tagvec.begin();
         i != tagvec.end(); ++i) {
        // std::cout<<*i<<' ';
        auto queryTag =
            select(all_of(tabTags)).from(tabTags).where(tabTags.title == *i);
        auto resultTag = pool->get_connection()(queryTag);
        // const auto& row = resultTag.front();
        // std::cout<<row.id<<"AAAAAAAAAAAAAa";
        PKey tagID = 0;
        if (!resultTag.empty()) {
            const auto& row = resultTag.front();
            tagID = row.id;
        } else {
            auto newid =
                pool->get_connection()(sqlpp::postgresql::insert_into(tabTags)
                                           .set(tabTags.title = *i)
                                           .returning(tabTags.id));
            tagID = newid.front().id.value();
        }
        auto newid = pool->get_connection()(
            sqlpp::postgresql::insert_into(tabTagTweet)
                .set(tabTagTweet.tweetID = tweetID, tabTagTweet.tagID = tagID)
                .returning(tabTagTweet.id));
    }
    return;
}
