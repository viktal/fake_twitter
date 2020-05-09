#include <sqlpp11/custom_query.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/sqlpp_models/TweetsTab.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"

#ifdef SQLPP_USE_SQLCIPHER
#include <sqlcipher/sqlite3.h>
#else

#include <sqlite3.h>

#endif

#include <cassert>
#include <iostream>
#include <vector>

namespace sql = sqlpp::sqlite3;
using fake_twitter::sqlpp_models::TabUsers;
using fake_twitter::sqlpp_models::TabTweets;
using fake_twitter::sqlpp_models::TabComments;

int main() {
    sql::connection_config config;
//    config.path_to_database = ":memory:";
    config.path_to_database = "/tmp/db.sqlite";
    config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    config.debug = true;

    sql::connection db(config);
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
    /*db.execute("CREATE TABLE Users (\n"
               "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
               "\tname varchar,\n"
               "\tusername varchar,\n"
               "\tpassword_hash integer,\n"
               "\tavatar string,\n"
               "\tfollowers_count integer,\n"
               "\tfriends_count integer\n"
               ");\n");*/

    /*db.execute("CREATE TABLE Tweets (\n"
               "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
               "\tbody string,\n"
               "\tauthor integer,\n"
               "\tcreate_date datetime,\n"
               "\trating integer,\n"
               "\tretweets integer\n"
               ");\n");*/

    db.execute("CREATE TABLE Comments (\n"
               "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
               "\tbody text,\n"
               "\tcreate_date datetime,\n"
               "\trating integer,\n"
               "\tauthor string,\n"
               "\tcomment_for integer\n"
               ");\n");

    TabUsers tabUsers;
    db(insert_into(tabUsers).set(
            tabUsers.name = "twitter",
            tabUsers.username = "twitter",
            tabUsers.password_hash = 123,
            tabUsers.friends_count = 0,
            tabUsers.followers_count = 0,
            tabUsers.avatar = "path"));

    for (const auto &row : db(select(all_of(tabUsers)).from(tabUsers).unconditionally())) {
        std::cout << row.id << " " <<
        row.name << " " <<
        row.username << " " <<
        row.avatar << " " <<
        row.followers_count << " " <<
        row.friends_count << " " <<
        row.password_hash << " " << std::endl;
    };

    TabTweets tabTweets;
    db(insert_into(tabTweets).set(
            tabTweets.body = "twittertwittertwitter",
            tabTweets.create_date = std::chrono::system_clock::now(),
            tabTweets.author = 1,
            tabTweets.retweets = 0,
            tabTweets.rating = 0));

    for (const auto &row : db(select(all_of(tabTweets)).from(tabTweets).unconditionally())) {
        std::cout << row.id << " " <<
        row.body << " " <<
        row.create_date << " " <<
        row.author << " " <<
        row.retweets << " " <<
        row.rating << " " << std::endl;
    };

    TabComments tabComments;
    db(insert_into(tabComments).set(
            tabComments.body = "lublu igrat v dotu 2",
            tabComments.create_date = std::chrono::system_clock::now(),
            tabComments.author = 1,
            tabComments.comment_for = 1,
            tabComments.rating = 0));

    for (const auto &row : db(select(all_of(tabComments)).from(tabComments).unconditionally())) {
        std::cout << row.id << " " <<
                  row.body << " " <<
                  row.create_date << " " <<
                  row.author << " " <<
                  row.comment_for << " " <<
                  row.rating << " " << std::endl;
    };
}


