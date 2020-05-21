#pragma once

#include "TestRestServer.h"
#include "fake_twitter/fake.h"
#include "gtest/gtest.h"

using namespace Pistache;
using namespace fake_twitter;

std::vector<model::User> make_users(Http::Client& client, int N) {
    std::vector<Async::Promise<Http::Response>> responses;
    std::vector<model::User> insertedUsers;
    std::mutex lock;

    // create random users
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/create?";
    for (int i = 0; i < N; i++) {
        auto userToInsert = fake::user::object();
        auto posturl = url + "username=" + userToInsert.username +
                       "&name=" + userToInsert.name;
        auto response = client.post(posturl).send();
        response.then(
            [userToInsert, &lock, &insertedUsers](Http::Response rsp) {
                EXPECT_EQ(rsp.code(), Http::Code::Ok);
                auto insertedUser =
                    serialization::from_json<model::User>(rsp.body());
                EXPECT_EQ(insertedUser.name, userToInsert.name);
                EXPECT_EQ(insertedUser.username, userToInsert.username);

                std::lock_guard<std::mutex> guard(lock);
                insertedUsers.push_back(std::move(insertedUser));
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
    return insertedUsers;
}

void update_users(Http::Client& client, std::vector<model::User>& users) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/update?";

    for (auto& user : users) {
        auto puturl = url + "id=" + std::to_string(user.id);
        if (rand() % 2) {
            user.name = fake::user::name();
            puturl += "&name=" + user.name;
        }
        //        if (rand() % 2) {
        //            user.username = fake::user::username();
        //            puturl += "&username=" + user.username;
        //        }
        auto response = client.put(puturl).send();
        response.then(
            [&](Http::Response rsp) { EXPECT_EQ(rsp.code(), Http::Code::Ok); },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

void select_users(Http::Client& client, const std::vector<model::User>& users,
                  bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;

    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/show?";

    for (auto& user : users) {
        auto geturl = url + "id=" + std::to_string(user.id);
        auto response = client.get(geturl).send();
        response.then(
            [user, &expect_fail](Http::Response rsp) {
                if (expect_fail)
                    EXPECT_EQ(rsp.code(), Http::Code::Bad_Request);
                else {
                    EXPECT_EQ(rsp.code(), Http::Code::Ok);
                    auto dbUser =
                        serialization::from_json<model::User>(rsp.body());
                    EXPECT_EQ(dbUser, user);
                }
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

void drop_users(Http::Client& client, const std::vector<model::User>& users,
                bool expect_fail) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/drop?";

    for (auto& user : users) {
        auto delurl = url + "id=" + std::to_string(user.id);
        auto response = client.del(delurl).send();
        response.then(
            [&](Http::Response rsp) {
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
