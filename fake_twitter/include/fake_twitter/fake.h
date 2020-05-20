#pragma once

#include <sqlpp11/postgresql/postgresql.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>
//#include <sqlpp11/sqlite3/sqlite3.h>

#include "fake_twitter/common.h"
#include "fake_twitter/model/User.h"

namespace fake_twitter::fake {
namespace user {
static auto rnd = std::mt19937(123);

std::string name() {
    static std::vector<std::string> names = {"Christopher",
                                             "JasmineGreen",
                                             "Steven",
                                             "Laurie",
                                             "PaulaHenson",
                                             "TimothyMcpherson",
                                             "Mary",
                                             "Chelsea",
                                             "BryanSmith",
                                             "AdamHordan",
                                             "Regina",
                                             "LisaAlvarez",
                                             "Megan",
                                             "Edgar",
                                             "EricaWebb",
                                             "Eric",
                                             "Justin",
                                             "DerekMacdonald",
                                             "Dr.LarryRichards",
                                             "James",
                                             "Brianna",
                                             "DanielHaynes",
                                             "ElizabethLee",
                                             "Melissa",
                                             "Angela",
                                             "MatthewReid",
                                             "LaurenColeman",
                                             "Aaron",
                                             "John",
                                             "DavidGriffin",
                                             "Paul",
                                             "DavidBrown",
                                             "ElizabethWalsh",
                                             "Charlotte",
                                             "LoriWhite",
                                             "Tamara",
                                             "Shannon",
                                             "Mrs.",
                                             "MichelleBoyer",
                                             "JohnAguilar",
                                             "Keith",
                                             "LuisWright",
                                             "JohnRogers",
                                             "Kevin",
                                             "Patricia",
                                             "DannyPrice",
                                             "TimAvila",
                                             "KimberlyHarrison",
                                             "VickiAdams",
                                             "Stephanie",
                                             "Darren",
                                             "VickieLucero",
                                             "Philip",
                                             "JessicaPatel",
                                             "Marissa",
                                             "JesseArias",
                                             "MaryArnold",
                                             "MatthewMorales",
                                             "ChristineFreeman",
                                             "ChristinaNguyen",
                                             "ThomasMosley",
                                             "Alex",
                                             "Amber",
                                             "NicoleMaxwell",
                                             "Alan",
                                             "Cody",
                                             "NicoleAlvarez",
                                             "CassandraPham",
                                             "DanielleNorman",
                                             "WilliamBarrett",
                                             "MarkMurphy",
                                             "KeithRoy",
                                             "MelissaLopez",
                                             "Shirley",
                                             "Anthony",
                                             "Spencer",
                                             "Matthew",
                                             "Kimberly",
                                             "Brandon",
                                             "JoseTerrell",
                                             "Daniel",
                                             "ReginaldLewis",
                                             "SusanWillis",
                                             "JenniferLewis",
                                             "BrandiSantos",
                                             "MathewRichmond",
                                             "Brittney",
                                             "MariaCampbell",
                                             "Savannah",
                                             "Jeffery",
                                             "Timothy",
                                             "Amanda",
                                             "Jessica",
                                             "TravisRichards",
                                             "Andrea",
                                             "FrancesNicholson",
                                             "Kenneth"};
    static std::uniform_int_distribution<int> nameIndSampler(0,
                                                             names.size() - 1);
    return names[nameIndSampler(rnd)];
}

std::string username() {
    static std::string alphanum =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static std::uniform_int_distribution<int> usernameSizeSampler(2, 25);

    std::string username;

    auto num = usernameSizeSampler(rnd);
    for (int i = 0; i < num; i++) {
        static std::uniform_int_distribution<int> alphanumIndSampler(
            0, alphanum.size() - 1);
        username += alphanum[alphanumIndSampler(rnd)];
    }
    //            std::sample(
    //                    alphanum.begin(), alphanum.end(),
    //                    std::back_inserter(username),
    //                    usernameSizeSampler(rnd), rnd
    //            );
    return username;
}

PasswordHash hash() {
    static std::uniform_int_distribution<PasswordHash> hashSampler(
        std::numeric_limits<PasswordHash>::min(),
        std::numeric_limits<PasswordHash>::max());
    return hashSampler(rnd);
}

model::User object() {
    // TODO придумать, что делать с ID
    return model::User{0, name(), username(), hash()};
}

}  // namespace user

namespace tweet {
    static auto rnd = std::mt19937(123);

std::string body() {
    std::string body;
    static std::string alphanum =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static std::uniform_int_distribution<int> bodySizeSampler(5, 250);
    auto num = bodySizeSampler(rnd);
    for (int i = 0; i < num; i++) {
        static std::uniform_int_distribution<int> alphanumIndSampler(
            0, alphanum.size() - 1);
        body += alphanum[alphanumIndSampler(rnd)];
    }
    return body;
}
    model::Tweet object() {
    // TODO придумать, что делать с ID
    return model::Tweet{0, body(), 0};
}
}

} // namespace tweet

void postgresql_tables(sqlpp::postgresql::connection& db) {
    db.execute(
        "CREATE TABLE Users (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"name\" varchar(255) NOT NULL,\n"
        "        \"username\" varchar(255) NOT NULL UNIQUE,\n"
        "        \"password_hash\" integer NOT NULL,\n"
        "        \"avatar\" VARCHAR(255) NOT NULL,\n"
        "        \"followers_count\" integer NOT NULL DEFAULT '0',\n"
        "        \"friends_count\" integer NOT NULL DEFAULT '0',\n"
        "        CONSTRAINT \"Users_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "CREATE TABLE Tweets (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"body\" VARCHAR(255) NOT NULL,\n"
        "        \"author\" integer NOT NULL,\n"
        "        \"create_date\" TIMESTAMP NOT NULL,\n"
        "        \"rating\" integer NOT NULL,\n"
        "        \"retweets\" integer NOT NULL,\n"
        "        CONSTRAINT \"Tweets_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "CREATE TABLE Comments (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"body\" TEXT NOT NULL,\n"
        "        \"create_date\" TIMESTAMP NOT NULL,\n"
        "        \"rating\" integer NOT NULL,\n"
        "        \"author\" integer NOT NULL,\n"
        "        \"comment_for\" integer NOT NULL,\n"
        "        CONSTRAINT \"Comments_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "CREATE TABLE UserFollower (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"UserID\" integer NOT NULL,\n"
        "        \"FollowsOn\" integer NOT NULL,\n"
        "        CONSTRAINT \"UserFollower_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "CREATE TABLE Tag (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"title\" VARCHAR(30) NOT NULL,\n"
        "        CONSTRAINT \"Tag_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "CREATE TABLE TagTweet (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"tweetID\" integer NOT NULL,\n"
        "        \"tagID\" integer NOT NULL,\n"
        "        CONSTRAINT \"TagTweet_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "CREATE TABLE LikeTweet (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"userID\" integer NOT NULL,\n"
        "        \"tweetID\" integer NOT NULL,\n"
        "        CONSTRAINT \"LikeTweet_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "ALTER TABLE Tweets ADD CONSTRAINT \"Tweets_fk0\" FOREIGN KEY "
        "(\"author\") REFERENCES Users(\"id\");\n"
        "\n"
        "ALTER TABLE Comments ADD CONSTRAINT \"Comments_fk0\" FOREIGN KEY "
        "(\"author\") REFERENCES Users(\"id\");\n"
        "ALTER TABLE Comments ADD CONSTRAINT \"Comments_fk1\" FOREIGN KEY "
        "(\"comment_for\") REFERENCES Tweets(\"id\");\n"
        "\n"
        "ALTER TABLE UserFollower ADD CONSTRAINT \"UserFollower_fk0\" FOREIGN "
        "KEY (\"UserID\") REFERENCES Users(\"id\");\n"
        "ALTER TABLE UserFollower ADD CONSTRAINT \"UserFollower_fk1\" FOREIGN "
        "KEY (\"FollowsOn\") REFERENCES Users(\"id\");\n"
        "\n"
        "\n"
        "ALTER TABLE TagTweet ADD CONSTRAINT \"TagTweet_fk0\" FOREIGN KEY "
        "(\"tweetID\") REFERENCES Tweets(\"id\");\n"
        "ALTER TABLE TagTweet ADD CONSTRAINT \"TagTweet_fk1\" FOREIGN KEY "
        "(\"tagID\") REFERENCES Tag(\"id\");\n"
        "\n"
        "ALTER TABLE LikeTweet ADD CONSTRAINT \"LikeTweet_fk0\" FOREIGN KEY "
        "(\"userID\") REFERENCES Tweets(\"id\");\n"
        "ALTER TABLE LikeTweet ADD CONSTRAINT \"LikeTweet_fk1\" FOREIGN KEY "
        "(\"tweetID\") REFERENCES Users(\"id\");\n"
        "");
}

//    void sqlite3tables(std::string path_to_db, bool debug = false) {
//        sqlpp::sqlite3::connection_config config;
//        config.path_to_database = path_to_db;
//        config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
//        config.debug = debug;
//        sqlpp::sqlite3::connection db(config);
//        sqlite3tables(db);
//    }

void postgresql_tables(
    const std::shared_ptr<sqlpp::postgresql::connection_config>& config) {
    sqlpp::postgresql::connection db(config);
    postgresql_tables(db);
}
}  // namespace fake_twitter::fake