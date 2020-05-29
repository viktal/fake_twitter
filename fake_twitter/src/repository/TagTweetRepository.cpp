#include "fake_twitter/repository/TagTweetRepository.h"

using namespace fake_twitter;
using fake_twitter::repository::TagTweetRepository;

TagTweetRepository::TagTweetRepository(
    std::shared_ptr<DBConnectionsPool> pool) {
    this->pool = std::move(pool);
}

std::unique_ptr<model::TagTweet> TagTweetRepository::get(PKey id) {
    auto query = select(all_of(tabTagTweet))
                     .from(tabTagTweet)
                     .where(tabTagTweet.id == id);

    auto result = pool->get_connection()(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::TagTweet> tagtweet;
    tagtweet = std::make_unique<model::TagTweet>(model::TagTweet{
        first.id.value(), first.tweetID.value(), first.tagID.value()});

    return tagtweet;
}

model::TagTweet TagTweetRepository::create(const PKey& tweetID,
                                           const PKey& tagID) {
    auto newid = pool->get_connection()(
        sqlpp::postgresql::insert_into(tabTagTweet)
            .set(tabTagTweet.tweetID = tweetID, tabTagTweet.tagID = tagID)
            .returning(tabTagTweet.id));

    return std::move(
        model::TagTweet{PKey(newid.front().id.value()), tweetID, tagID});
}