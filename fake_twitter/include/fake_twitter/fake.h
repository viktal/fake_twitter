#pragma once

#include <sqlpp11/postgresql/postgresql.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>
//#include <sqlpp11/sqlite3/sqlite3.h>

#include "fake_twitter/common.h"
#include "fake_twitter/model/Comment.h"
#include "fake_twitter/model/Tweet.h"
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
    static std::uniform_int_distribution<PasswordHash> hashSampler(0, 100000);
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
    static std::vector<std::string> words ={"Name",
                                             "create",
                                             "fake",
                                             "free",
                                             "foo",
                                             "drake",
                                             "male",
                                             "xxx",
                                             "gg",
                                             "ruin",
                                             "team",
                                             "fake",
                                             "gg",
                                             "csgo",
                                             "tree",
                                             "polindro,",
                                             "algorithm",
                                             "c++",
                                             "you",
                                             "aaaaa",
                                             "traightor",
                                             "fake",
                                             "trash",
                                             "rating",
                                             "group",
                                             "discord",
                                             "tab",
                                             "alright",
                                             "train",
                                             "gabe",
                                             "create",
                                             "delete",
                                             "get",
                                             "report",
                                             "compendium",
                                             "january",
                                             "december",
                                             "track",
                                             "song",
                                             "Nike",
                                             "Jordan",
                                             "Adidas",
                                             "cat",
                                             "dog",
                                             "pacific",
                                             "pen",
                                             "amigo",
                                             "rioted",
                                             "wagons",
                                             "payors",
                                             "nantle",
                                             "oxshoe",
                                             "yolked",
                                             "parkee",
                                             "vespal",
                                             "peages",
                                             "siryan",
                                             "worldwide",
                                             "impressive",
                                             "ideal",
                                             "cold",
                                             "significant",
                                             "sport",
                                             "success",
                                             "puppy",
                                             "friend",
                                             "rainbow",
                                             "grandiose",
                                             "greatest",
                                             "strong",
                                             "football",
                                             "stork",
                                             "team",
                                             "stadium",
                                             "gift"};
    static std::uniform_int_distribution<int> bodySizeSampler(2, 10);
    auto num = bodySizeSampler(rnd);
    for (int i = 0; i < num; i++) {
        static std::uniform_int_distribution<int> bodyIndSampler(
            0, words.size() - 1);
        static std::uniform_int_distribution<int> bodyHashSampler(
            0, 2);
        if (bodyHashSampler(rnd) % 3 == 0) {
            body += "#";
        }
        body += words[bodyIndSampler(rnd)];
        body += " ";
    }
    return body;
}
model::Tweet object(int userCount) {
    static std::uniform_int_distribution<int> userCountSizeSampler(1, userCount);
    int num = userCountSizeSampler(rnd);
    return model::Tweet{0, body(), num};
}

}  // namespace tweet

namespace comment {
static auto rnd = std::mt19937(123);

std::string body() {
    std::string body;
    static std::vector<std::string> words ={"Name",
                                             "create",
                                             "fake",
                                             "free",
                                             "foo",
                                             "drake",
                                             "male",
                                             "xxx",
                                             "gg",
                                             "ruin",
                                             "team",
                                             "fake",
                                             "gg",
                                             "csgo",
                                             "tree",
                                             "polindro,",
                                             "algorithm",
                                             "c++",
                                             "you",
                                             "aaaaa",
                                             "traightor",
                                             "fake",
                                             "trash",
                                             "rating",
                                             "group",
                                             "discord",
                                             "tab",
                                             "alright",
                                             "train",
                                             "gabe",
                                             "create",
                                             "delete",
                                             "get",
                                             "report",
                                             "compendium",
                                             "january",
                                             "december",
                                             "track",
                                             "song",
                                             "Nike",
                                             "Jordan",
                                             "Adidas",
                                             "cat",
                                             "dog",
                                             "pacific",
                                             "pen",
                                             "amigo",
                                             "rioted",
                                             "wagons",
                                             "payors",
                                             "nantle",
                                             "oxshoe",
                                             "yolked",
                                             "parkee",
                                             "vespal",
                                             "peages",
                                             "siryan",
                                             "worldwide",
                                             "impressive",
                                             "ideal",
                                             "cold",
                                             "significant",
                                             "sport",
                                             "success",
                                             "puppy",
                                             "friend",
                                             "rainbow",
                                             "grandiose",
                                             "greatest",
                                             "strong",
                                             "football",
                                             "stork",
                                             "team",
                                             "stadium",
                                             "gift"};
    static std::uniform_int_distribution<int> bodySizeSampler(2, 10);
    auto num = bodySizeSampler(rnd);
    for (int i = 0; i < num; i++) {
        static std::uniform_int_distribution<int> bodyIndSampler(
            0, words.size() - 1);
        static std::uniform_int_distribution<int> bodyHashSampler(
            0, 2);
        if (bodyHashSampler(rnd) % 3 == 0) {
            body += "#";
        }
        body += words[bodyIndSampler(rnd)];
        body += " ";
    }
    return body;
}
model::Comment object(int userCount, int tweetCount) {
    static std::uniform_int_distribution<int> userCountSizeSampler(1, userCount);
    int num = userCountSizeSampler(rnd);
    static std::uniform_int_distribution<int> tweetCountSizeSampler(1, tweetCount);
    int num1 = tweetCountSizeSampler(rnd);
    model::Comment comment;
    comment.set(0, body(), num, num1);
    return comment;
}

}  // namespace comment

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
        "CREATE TABLE Follower (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"author\" integer NOT NULL,\n"
        "        \"addresser\" integer NOT NULL,\n"
        "        CONSTRAINT \"Follower_pk\" PRIMARY KEY (\"id\")\n"
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
        "CREATE TABLE Likes (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"author\" integer NOT NULL,\n"
        "        \"twit\" integer NOT NULL,\n"
        "        CONSTRAINT \"LikeTweet_pk\" PRIMARY KEY (\"id\")\n"
        ") WITH (\n"
        "  OIDS=FALSE\n"
        ");\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "ALTER TABLE Tweets ADD CONSTRAINT \"Tweets_fk0\" FOREIGN KEY "
        "(\"author\") REFERENCES Users(\"id\") ON DELETE CASCADE;\n"
        "\n"
        "ALTER TABLE Comments ADD CONSTRAINT \"Comments_fk0\" FOREIGN KEY "
        "(\"author\") REFERENCES Users(\"id\") ON DELETE CASCADE;\n"
        "ALTER TABLE Comments ADD CONSTRAINT \"Comments_fk1\" FOREIGN KEY "
        "(\"comment_for\") REFERENCES Tweets(\"id\") ON DELETE CASCADE;\n"
        "\n"
        "ALTER TABLE Follower ADD CONSTRAINT \"Follower_fk0\" FOREIGN "
        "KEY (\"author\") REFERENCES Users(\"id\") ON DELETE CASCADE;\n"
        "ALTER TABLE Follower ADD CONSTRAINT \"Follower_fk1\" FOREIGN "
        "KEY (\"addresser\") REFERENCES Users(\"id\") ON DELETE CASCADE;\n"
        "\n"
        "\n"
        "ALTER TABLE TagTweet ADD CONSTRAINT \"TagTweet_fk0\" FOREIGN KEY "
        "(\"tweetID\") REFERENCES Tweets(\"id\") ON DELETE CASCADE;\n"
        "ALTER TABLE TagTweet ADD CONSTRAINT \"TagTweet_fk1\" FOREIGN KEY "
        "(\"tagID\") REFERENCES Tag(\"id\") ON DELETE CASCADE;\n"
        "\n"
        "ALTER TABLE Likes ADD CONSTRAINT \"Likes_fk0\" FOREIGN KEY "
        "(\"author\") REFERENCES Users(\"id\") ON DELETE CASCADE;\n"
        "ALTER TABLE Likes ADD CONSTRAINT \"Likes_fk1\" FOREIGN KEY "
        "(\"twit\") REFERENCES Tweets(\"id\") ON DELETE CASCADE;\n"
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