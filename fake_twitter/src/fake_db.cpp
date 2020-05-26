#include <sqlpp11/custom_query.h>
#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/sqlpp11.h>

#include <iostream>

#include "fake_twitter/fake.h"
#include "fake_twitter/sqlpp_models/TagTweetTab.h"
#include "fake_twitter/sqlpp_models/TagsTab.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"
#include "fake_twitter/sqlpp_models/FollowerTab.h"
#include "fake_twitter/sqlpp_models/LikeTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"

namespace sql = sqlpp::postgresql;
using fake_twitter::sqlpp_models::TabTagTweet;
using fake_twitter::sqlpp_models::TabTags;
using fake_twitter::sqlpp_models::TabComments;
using fake_twitter::sqlpp_models::TabFollower;
using fake_twitter::sqlpp_models::TabLikes;
using fake_twitter::sqlpp_models::TabTweets;
using fake_twitter::sqlpp_models::TabUsers;

int main() {
    auto config = std::make_shared<sqlpp::postgresql::connection_config>();

    //    config.path_to_database = ":memory:";
    config->host = "127.0.0.1";
    config->user = "twituser";
    config->password = "123";
    config->dbname = "twitdb";
    //    config.options
    //    config.path_to_database = "/tmp/db.sqlite";
    //    config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    config->debug = true;
    sqlpp::postgresql::connection db(config);
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
    fake_twitter::fake::postgresql_tables(db);

    static auto rnd = std::mt19937(123);
    int userCount = 10;
    int tweetCount = 10;
    int commentCount = 10;

    TabUsers tabUsers;
    for (int i = 0; i < userCount; i++) {
        auto user = fake_twitter::fake::user::object();
        db(insert_into(tabUsers).set(
            tabUsers.name = user.name, tabUsers.username = user.username,
            tabUsers.password_hash = user.password_hash,
            tabUsers.friends_count = 0, tabUsers.followers_count = 0,
            tabUsers.avatar = "path"));
    }

    TabTweets tabTweets;
    for (int i = 0; i < tweetCount; i++) {
        auto tweet = fake_twitter::fake::tweet::object(userCount);
        db(insert_into(tabTweets).set(
            tabTweets.body = tweet.body,
            tabTweets.create_date = std::chrono::system_clock::now(),
            tabTweets.author = tweet.author, tabTweets.retweets = 0,
            tabTweets.rating = 0));
    }


    TabFollower tabFollower;
    for (int i = 0; i <= userCount*userCount; i++) {
        static std::uniform_int_distribution<int> userSizeSampler(1, userCount);
        int num1 = userSizeSampler(rnd);
        int num2 = userSizeSampler(rnd);
        if (num1 != num2) {
            db(insert_into(tabFollower)
                    .set(tabFollower.author = num1, tabFollower.addresser = num2));
            //fake_twitter::repository::UsersRepository::follow(num1, num2);
        }
    }

    TabComments tabComments;
    for (int i = 0; i < commentCount; i++) {
        auto comment = fake_twitter::fake::comment::object(userCount, tweetCount);
        db(insert_into(tabComments)
            .set(tabComments.body = comment.body,
                    tabComments.create_date = std::chrono::system_clock::now(),
                    tabComments.author = comment.author, tabComments.comment_for = comment.comment_for,
                    tabComments.rating = 0));
    }

    TabTags tabTags;
    db(insert_into(tabTags)
            .set(tabTags.title = "dota"));

    TabTagTweet tabTagTweet;
    db(insert_into(tabTagTweet)
            .set(tabTagTweet.tweetID = 2, tabTagTweet.tagID = 1));


    /*db(insert_into(tabUsers).set(
        tabUsers.name = "twitter", tabUsers.username = "twitter",
        tabUsers.password_hash = 123, tabUsers.friends_count = 0,
        tabUsers.followers_count = 0, tabUsers.avatar = "path"));

    TabTweets tabTweets;
    db(insert_into(tabTweets).set(
        tabTweets.body = "twittertwittertwitter",
        tabTweets.create_date = std::chrono::system_clock::now(),
        tabTweets.author = 1, tabTweets.retweets = 0, tabTweets.rating = 0));

    db(insert_into(tabTweets).set(
        tabTweets.body = "twittertwittertwitter2",
        tabTweets.create_date = std::chrono::system_clock::now(),
        tabTweets.author = 1, tabTweets.retweets = 0, tabTweets.rating = 0));

    TabComments tabComments;
    db(insert_into(tabComments)
           .set(tabComments.body = "lublu igrat v dotu 2",
                tabComments.create_date = std::chrono::system_clock::now(),
                tabComments.author = 1, tabComments.comment_for = 1,
                tabComments.rating = 0));

    for (const auto& row :
         db(select(all_of(tabComments)).from(tabComments).unconditionally())) {
        std::cout << row.id << " " << row.body << " " << row.create_date << " "
                  << row.author << " " << row.comment_for << " " << row.rating
                  << " " << std::endl;
    };
    db(insert_into(tabComments)
           .set(tabComments.body = "Why?",
                tabComments.create_date = std::chrono::system_clock::now(),
                tabComments.author = 1, tabComments.comment_for = 2,
                tabComments.rating = 0));

    for (const auto& row :
         db(select(all_of(tabComments)).from(tabComments).unconditionally())) {
        std::cout << row.id << " " << row.body << " " << row.create_date << " "
                  << row.author << " " << row.comment_for << " " << row.rating
                  << " " << std::endl;
    };

    TabFollower tabFollower;
        db(insert_into(tabFollower).set(
                tabFollower.author = 1,
               tabFollower.addresser = 2));

    TabLikes tabLike;
        db(insert_into(tabLike).set(
                tabLike.author = 1,
                tabLike.twit = 2));*/
}
