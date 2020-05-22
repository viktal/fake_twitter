#pragma once

#include "TestRestServer.h"
#include "fake_twitter/fake.h"
#include "gtest/gtest.h"

using namespace Pistache;
using namespace fake_twitter;
static auto rnd = std::mt19937(123);

std::vector<model::Tweet> make_tweets(Http::Client& client,
                                      std::vector<model::User> users, int N) {
    std::vector<Async::Promise<Http::Response>> responses;
    std::vector<model::Tweet> insertedTweets;
    std::mutex lock;

    const std::string url = "http://" + ADDRESS + ":" + std::to_string(PORT) +
                            "/0.0/tweets/create?";
    for (const auto& user : users) {
        static std::uniform_int_distribution<int> tweetsCountSampler(0, N);
        auto count = tweetsCountSampler(rnd);
        for (int i = 0; i < count; i++) {
            auto tweetToInsert = fake::tweet::object(user.id);
            auto posturl =
                url + "author=" + std::to_string(tweetToInsert.author);
            std::string data = R"({"body":")" + tweetToInsert.body + "\"}";
            auto response = client.post(posturl).body(data).send();
            response.then(
                [tweetToInsert, &lock, &insertedTweets](Http::Response rsp) {
                    EXPECT_EQ(rsp.code(), Http::Code::Ok);
                    auto insertedTweet =
                        serialization::from_json<model::Tweet>(rsp.body());
                    EXPECT_EQ(insertedTweet.body, tweetToInsert.body);
                    EXPECT_EQ(insertedTweet.author, tweetToInsert.author);
                    std::lock_guard<std::mutex> guard(lock);
                    insertedTweets.push_back(std::move(insertedTweet));
                },
                onfail);
            responses.push_back(std::move(response));
        }
        awaitall(responses);
    }
    return insertedTweets;
}

void select_tweets(Http::Client& client,
                   const std::vector<model::Tweet>& tweets, bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/tweets/show?";

    for (const auto& tweet : tweets) {
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
    awaitall(responses);
}
void drop_tweets(Http::Client& client, std::vector<model::Tweet> tweets,
                 bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/tweets/drop?";

    for (const auto& tweet : tweets) {
        auto deleteurl = url + "id=" + std::to_string(tweet.id);
        auto response = client.del(deleteurl).send();
        response.then(
            [tweet, expect_fail](Http::Response rsp) {
                if (expect_fail)
                    EXPECT_EQ(rsp.code(), Http::Code::Bad_Request);
                else
                    EXPECT_EQ(rsp.code(), Http::Code::Ok);
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}