#include <sqlpp11/custom_query.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/sqlpp_models/UsersTab.h"

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

int main() {
    sql::connection_config config;
//    config.path_to_database = ":memory:";
    config.path_to_database = "/tmp/db.sqlite";
    config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    config.debug = true;

    sql::connection db(config);
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
    db.execute("CREATE TABLE Users (\n"
               "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
               "\tname varchar,\n"
               "\tusername varchar,\n"
               "\tpassword_hash integer,\n"
               "\tavatar string,\n"
               "\tfollowers_count integer,\n"
               "\tfriends_count integer\n"
               ");\n");

    TabUsers tab;
    // explicit all_of(tab)
    db(insert_into(tab).set(
            tab.name = "twitter",
            tab.username = "twitter",
            tab.password_hash = 123,
            tab.friends_count = 0,
            tab.followers_count = 0,
            tab.avatar = "path"));

    for (const auto &row : db(select(all_of(tab)).from(tab).unconditionally())) {
        std::cout << row.id << " " <<
        row.name << " " <<
        row.username << " " <<
        row.avatar << " " <<
        row.followers_count << " " <<
        row.friends_count << " " <<
        row.password_hash << " " << std::endl;
    };
}