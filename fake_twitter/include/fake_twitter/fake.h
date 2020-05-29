#pragma once

#include <sqlpp11/postgresql/postgresql.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "fake_twitter/common.h"
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/model/User.h"
#include "fake_twitter/utils.h"

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

model::User object(const PKey& id = 0, const std::string& postfix = "",
                   const std::string& password = "") {
    PasswordHash pswdhash;
    std::string salt = utils::salt();
    if (password.size() != 0)
        pswdhash = utils::make_password_hash(salt, password);
    else
        pswdhash = hash();
    return model::User{
        0, name() + postfix, username() + postfix, pswdhash, salt, 0, 0};
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
model::Tweet object(int userCount) {
    static std::uniform_int_distribution<int> bodySizeSampler(1, userCount);
    int num = bodySizeSampler(rnd);
    return model::Tweet{0, body(), num};
}

}  // namespace tweet

void create_postgresql_tables(sqlpp::postgresql::connection& db) {
    db.execute(
        "CREATE TABLE Users (\n"
        "        \"id\" serial NOT NULL,\n"
        "        \"name\" varchar(255) NOT NULL,\n"
        "        \"username\" varchar(255) NOT NULL UNIQUE,\n"
        "        \"password_hash\" bigint NOT NULL,\n"
        "        \"salt\" varchar(255) NOT NULL,\n"
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
        "ALTER TABLE Follower ADD CONSTRAINT \"Follower_fk0\" FOREIGN "
        "KEY (\"author\") REFERENCES Users(\"id\");\n"
        "ALTER TABLE Follower ADD CONSTRAINT \"Follower_fk1\" FOREIGN "
        "KEY (\"addresser\") REFERENCES Users(\"id\");\n"
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

void create_postgresql_tables(
    const std::shared_ptr<sqlpp::postgresql::connection_config>& config) {
    sqlpp::postgresql::connection db(config);
    create_postgresql_tables(db);
}

void drop_postgresql_tables(sqlpp::postgresql::connection& db) {
    db.execute("DROP TABLE IF EXISTS LikeTweet CASCADE;");
    db.execute("DROP TABLE IF EXISTS TagTweet CASCADE;");
    db.execute("DROP TABLE IF EXISTS Tag CASCADE;");
    db.execute("DROP TABLE IF EXISTS Follower CASCADE;");
    db.execute("DROP TABLE IF EXISTS Comments CASCADE;");
    db.execute("DROP TABLE IF EXISTS Tweets CASCADE;");
    db.execute("DROP TABLE IF EXISTS Users CASCADE;");
}

void drop_postgresql_tables(
    const std::shared_ptr<sqlpp::postgresql::connection_config>& config) {
    sqlpp::postgresql::connection db(config);
    drop_postgresql_tables(db);
}
}  // namespace fake_twitter::fake