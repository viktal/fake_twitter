#include <sqlpp11/custom_query.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <sqlpp11/sqlpp11.h>

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
using fake_twitter::sqlpp_models::TabComments;

int main() {
    sql::connection_config config;
//    config.path_to_database = ":memory:";
    config.path_to_database = "/tmp/db.sqlite";
    config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    config.debug = true;

    sql::connection db(config);
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;



    db.execute("CREATE TABLE Comments (\n"
               "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
               "\tbody text,\n"
               "\tcreate_date datetime,\n"
               "\trating integer,\n"
               "\tauthor string,\n"
               "\tcomment_for integer\n"
               ");\n");


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
    db(insert_into(tabComments).set(
            tabComments.body = "Why?",
            tabComments.create_date = std::chrono::system_clock::now(),
            tabComments.author = 1,
            tabComments.comment_for = 2,
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


