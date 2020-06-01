#include <sqlpp11/custom_query.h>
#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/sqlpp11.h>

#include <iostream>

#include "fake_twitter/fake.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"
#include "fake_twitter/sqlpp_models/FollowerTab.h"
#include "fake_twitter/sqlpp_models/LikeTab.h"
#include "fake_twitter/sqlpp_models/TagTweetTab.h"
#include "fake_twitter/sqlpp_models/TagsTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"

namespace sql = sqlpp::postgresql;
using fake_twitter::sqlpp_models::TabComments;
using fake_twitter::sqlpp_models::TabFollower;
using fake_twitter::sqlpp_models::TabLikes;
using fake_twitter::sqlpp_models::TabTags;
using fake_twitter::sqlpp_models::TabTagTweet;
using fake_twitter::sqlpp_models::TabTweets;
using fake_twitter::sqlpp_models::TabUsers;

int main() {
    auto config = std::make_shared<sqlpp::postgresql::connection_config>();
    config->host = "127.0.0.1";
    config->user = "twituser";
    config->password = "123";
    config->dbname = "twitdb";

    config->debug = true;
    sqlpp::postgresql::connection db(config);
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
    fake_twitter::fake::drop_postgresql_tables(db);
    fake_twitter::fake::create_postgresql_tables(db);

    static auto rnd = std::mt19937(123);
    int userCount = 10;
    int tweetCount = 10;
    int commentCount = 10;

    TabUsers tabUsers;
    for (int i = 0; i < userCount; i++) {
        auto password = std::to_string(i);
        auto user = fake_twitter::fake::user::object(i, password, password);
        db(insert_into(tabUsers).set(
            tabUsers.name = user.name, tabUsers.username = user.username,
            tabUsers.password_hash = user.password_hash,
            tabUsers.friends_count = 0, tabUsers.followers_count = 0,
            tabUsers.salt = user.salt));
    }

    TabTweets tabTweets;
    for (int i = 0; i < tweetCount; i++) {
        auto tweet = fake_twitter::fake::tweet_comment::object_tweet(userCount);
        db(insert_into(tabTweets).set(
            tabTweets.body = tweet.body,
            tabTweets.create_date = std::chrono::system_clock::now(),
            tabTweets.author = tweet.author, tabTweets.retweets = 0,
            tabTweets.rating = 0));
    }

    TabFollower tabFollower;
    int u = 0;
    while (u <= userCount * userCount / 2) {
        static std::uniform_int_distribution<int> userSizeSampler(1, userCount);
        int num1 = userSizeSampler(rnd);
        int num2 = userSizeSampler(rnd);
        if (num1 != num2) {
            auto buf = db(select(all_of(tabFollower))
                              .from(tabFollower)
                              .where(tabFollower.author == num1 &&
                                     tabFollower.addresser == num2));

            if (buf.size() == 0) {
                db(insert_into(tabFollower)
                       .set(tabFollower.author = num1,
                            tabFollower.addresser = num2));
                ++u;
            }
            // fake_twitter::repository::UsersRepository::follow(num1, num2);
        }
    }

    TabComments tabComments;
    for (int i = 0; i < commentCount; i++) {
        auto comment = fake_twitter::fake::tweet_comment::object_comment(
            userCount, tweetCount);
        db(insert_into(tabComments)
               .set(tabComments.body = comment.body,
                    tabComments.create_date = std::chrono::system_clock::now(),
                    tabComments.author = comment.author,
                    tabComments.comment_for = comment.comment_for,
                    tabComments.rating = 0));
    }

    TabLikes tabLikes;
    int us;
    while (us <= userCount * tweetCount / 2) {
        static std::uniform_int_distribution<int> userSizeSampler(1, userCount);
        static std::uniform_int_distribution<int> tweetSizeSampler(1,
                                                                   userCount);
        int num1 = userSizeSampler(rnd);
        int num2 = tweetSizeSampler(rnd);
        auto buf =
            db(select(all_of(tabLikes))
                   .from(tabLikes)
                   .where(tabLikes.author == num1 && tabLikes.twit == num2));

        if (buf.size() == 0) {
            db(insert_into(tabLikes).set(tabLikes.author = num1,
                                         tabLikes.twit = num2));
            db(update(tabTweets)
                   .set(tabTweets.rating = tabTweets.rating + 1)
                   .where(tabTweets.id == num2));
            //++us;
        }
        ++us;
    }

    TabTags tabTags;
    db(insert_into(tabTags).set(tabTags.title = "dota"));

    TabTagTweet tabTagTweet;
    db(insert_into(tabTagTweet)
           .set(tabTagTweet.tweetID = 2, tabTagTweet.tagID = 1));
}
