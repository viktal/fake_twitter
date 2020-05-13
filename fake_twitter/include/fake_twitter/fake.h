#pragma once

#include <sqlpp11/sqlite3/sqlite3.h>
#include "fake_twitter/model/User.h"

namespace fake_twitter::fake{
    model::User user () {
        return model::User{0, "user", "user", 123};
    }

    void tables (std::string path_to_db, bool debug = false) {
        sqlpp::sqlite3::connection_config config;
        config.path_to_database = path_to_db;
        config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        config.debug = debug;
        sqlpp::sqlite3::connection db(config);
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
    }
}
