#pragma once

#include "TestRestServer.h"
#include "fake_twitter/fake.h"
#include "gtest/gtest.h"
#include "users_crud_test.h"

using namespace Pistache;
using namespace fake_twitter;
static auto rnd = std::mt19937(123);

void make_tweets(Http::Client& client,
                 std::vector<UserCredentials>& credentials, int N) {
    std::vector<Async::Promise<Http::Response>> responses;
    std::vector<model::Tweet> insertedTweets;
    std::mutex lock;

    const std::string url = "http://" + ADDRESS + ":" + std::to_string(PORT) +
                            "/0.0/tweets/create?";
    for (auto& cred : credentials) {
        std::uniform_int_distribution<int> tweetsCountSampler(0, N);
        auto count = tweetsCountSampler(rnd);
        for (int i = 0; i < count; i++) {
            auto tweetToInsert = fake::tweet::object(20);
            tweetToInsert.author = cred.user.id;
            auto posturl =
                url + "author=" + std::to_string(tweetToInsert.author);
            std::string data = R"({"body":")" + tweetToInsert.body + "\"}";
            auto response =
                client.post(posturl).cookie(cred.session).body(data).send();
            response.then(
                [tweetToInsert, &lock, &cred](Http::Response rsp) {
                    EXPECT_EQ(rsp.code(), Http::Code::Ok);
                    auto insertedTweet =
                        serialization::from_json<model::Tweet>(rsp.body());
                    EXPECT_EQ(insertedTweet.body, tweetToInsert.body);
                    EXPECT_EQ(insertedTweet.author, tweetToInsert.author);
                    std::lock_guard<std::mutex> guard(lock);
                    cred.tweets.push_back(std::move(insertedTweet));
                },
                onfail);
            responses.push_back(std::move(response));
        }
        awaitall(responses);
    }
}

void select_tweets(Http::Client& client,
                   const std::vector<UserCredentials>& credential,
                   bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/tweets/show?";

    for (const auto& cred : credential) {
        for (const auto& tweet : cred.tweets) {
            auto geturl = url + "id=" + std::to_string(tweet.id);
            auto response = client.get(geturl).send();
            response.then(
                [tweet, &expect_fail](Http::Response rsp) {
                    if (expect_fail)
                        EXPECT_EQ(rsp.code(), Http::Code::Bad_Request);
                    else {
                        EXPECT_EQ(rsp.code(), Http::Code::Ok);
                        auto db_tweet =
                            serialization::from_json<model::Tweet>(rsp.body());
                        EXPECT_EQ(db_tweet, tweet);
                    }
                },
                onfail);
            responses.push_back(std::move(response));
        }
    }

    awaitall(responses);
}
void drop_tweets(Http::Client& client, std::vector<UserCredentials> credentials,
                 bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/tweets/drop?";

    for (const auto& cred: credentials) {
        for (const auto& tweet : cred.tweets) {
            auto deleteurl = url + "id=" + std::to_string(tweet.id);
            auto response =
                client.del(deleteurl).cookie(cred.session).send();
            response.then(
                [tweet, expect_fail](Http::Response rsp) {
                  if (expect_fail)
                      EXPECT_EQ(rsp.code(), Http::Code::Not_Found);
                  else
                      EXPECT_EQ(rsp.code(), Http::Code::Ok);
                },
                onfail);
            responses.push_back(std::move(response));
        }
    }

    awaitall(responses);
}