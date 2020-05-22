#include <sqlpp11/custom_query.h>
#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/sqlpp11.h>

#include <iostream>

#include "fake_twitter/fake.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"
#include "fake_twitter/sqlpp_models/FollowerTab.h"
#include "fake_twitter/sqlpp_models/LikeTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"

namespace sql = sqlpp::postgresql;
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

    int userCount = 10;
    int tweetCount = 10;

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

//    TabFollower tabFollower;
//    for (int i = 1; i <= userCount - 1; i++) {
//        for (int j = i + 1; j <= userCount; j++) {
//            db(insert_into(tabFollower)
//                   .set(tabFollower.author = i, tabFollower.addresser = j));
//        }
//    }

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
