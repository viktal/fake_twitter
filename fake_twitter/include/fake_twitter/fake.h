#pragma once

#include <vector>
#include <random>
#include <string>
#include <algorithm>

#include <sqlpp11/sqlite3/sqlite3.h>

#include "fake_twitter/model/User.h"
#include "fake_twitter/common.h"

namespace fake_twitter::fake {
    namespace user {
        static auto rnd = std::mt19937(123);

        std::string name() {
            static std::vector<std::string> names = {
                    "Christopher", "JasmineGreen", "Steven", "Laurie", "PaulaHenson", "TimothyMcpherson", "Mary",
                    "Chelsea", "BryanSmith", "AdamHordan", "Regina", "LisaAlvarez", "Megan", "Edgar", "EricaWebb",
                    "Eric", "Justin", "DerekMacdonald", "Dr.LarryRichards", "James", "Brianna", "DanielHaynes",
                    "ElizabethLee", "Melissa", "Angela", "MatthewReid", "LaurenColeman", "Aaron", "John",
                    "DavidGriffin", "Paul", "DavidBrown", "ElizabethWalsh", "Charlotte", "LoriWhite", "Tamara",
                    "Shannon", "Mrs.", "MichelleBoyer", "JohnAguilar", "Keith", "LuisWright", "JohnRogers", "Kevin",
                    "Patricia", "DannyPrice", "TimAvila", "KimberlyHarrison", "VickiAdams", "Stephanie", "Darren",
                    "VickieLucero", "Philip", "JessicaPatel", "Marissa", "JesseArias", "MaryArnold", "MatthewMorales",
                    "ChristineFreeman", "ChristinaNguyen", "ThomasMosley", "Alex", "Amber", "NicoleMaxwell", "Alan",
                    "Cody", "NicoleAlvarez", "CassandraPham", "DanielleNorman", "WilliamBarrett", "MarkMurphy",
                    "KeithRoy", "MelissaLopez", "Shirley", "Anthony", "Spencer", "Matthew", "Kimberly", "Brandon",
                    "JoseTerrell", "Daniel", "ReginaldLewis", "SusanWillis", "JenniferLewis", "BrandiSantos",
                    "MathewRichmond", "Brittney", "MariaCampbell", "Savannah", "Jeffery", "Timothy", "Amanda", "Jessica",
                    "TravisRichards", "Andrea", "FrancesNicholson", "Kenneth"
            };
            static std::uniform_int_distribution<int> nameIndSampler(0, names.size() - 1);
            return names[nameIndSampler(rnd)];
        }

        std::string username() {
            static std::string alphanum = "0123456789"
                                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                          "abcdefghijklmnopqrstuvwxyz";
            static std::uniform_int_distribution<int> usernameSizeSampler(2, 25);

            std::string username;
            std::sample(
                    alphanum.begin(), alphanum.end(), std::back_inserter(username),
                    usernameSizeSampler(rnd), rnd
            );
            return username;
        }

        PasswordHash hash() {
            static std::uniform_int_distribution<PasswordHash> hashSampler(
                    std::numeric_limits<PasswordHash>::min(), std::numeric_limits<PasswordHash>::max()
            );
            return hashSampler(rnd);
        }

        model::User object() {
            // TODO придумать, что делать с ID
            return model::User{0, name(), username(), hash()};
        }

    }

    void sqlite3tables(sqlpp::sqlite3::connection &db) {
        db.execute("CREATE TABLE Users (\n"
                   "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
                   "\tname varchar,\n"
                   "\tusername varchar,\n"
                   "\tpassword_hash integer,\n"
                   "\tavatar string,\n"
                   "\tfollowers_count integer,\n"
                   "\tfriends_count integer\n"
                   ");\n");

        db.execute("CREATE TABLE Tweets (\n"
                   "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
                   "\tbody string,\n"
                   "\tauthor integer,\n"
                   "\tcreate_date datetime,\n"
                   "\trating integer,\n"
                   "\tretweets integer\n"
                   ");\n");
    }

    void sqlite3tables(std::string path_to_db, bool debug = false) {
        sqlpp::sqlite3::connection_config config;
        config.path_to_database = path_to_db;
        config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        config.debug = debug;
        sqlpp::sqlite3::connection db(config);
        sqlite3tables(db);
    }
}