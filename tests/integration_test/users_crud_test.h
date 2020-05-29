#pragma once

#include "TestRestServer.h"
#include "fake_twitter/fake.h"
#include "gtest/gtest.h"

using namespace Pistache;
using namespace fake_twitter;

struct WorkloadInfo {
    explicit WorkloadInfo(std::string password) {
        this->user = fake::user::object(0, password, password);
        this->password = std::move(password);
    }
    WorkloadInfo(const model::User& user, const std::string& password) {
        this->user = user;
        this->password = password;
    }
    model::User user = model::User();
    std::vector<model::Tweet> tweets{};
    std::string password = "";
    Http::Cookie session = Http::Cookie("", "");
};

std::vector<WorkloadInfo> make_users(Http::Client& client, int N) {
    std::vector<Async::Promise<Http::Response>> responses;
    auto inserted = std::vector<WorkloadInfo>();
    std::mutex lock;

    // create random users
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/create?";
    for (int i = 0; i < N; i++) {
        auto to_insert = WorkloadInfo(std::to_string(i));
        auto posturl = url + "username=" + to_insert.user.username +
                       "&name=" + to_insert.user.name +
                       "&password=" + to_insert.password;
        auto response = client.post(posturl).send();
        response.then(
            [to_insert, &lock, &inserted](Http::Response rsp) {
                ASSERT_EQ(rsp.code(), Http::Code::Ok);
                auto inserted_user =
                    serialization::from_json<model::User>(rsp.body());
                ASSERT_EQ(inserted_user.name, to_insert.user.name);
                ASSERT_EQ(inserted_user.username, to_insert.user.username);

                std::lock_guard<std::mutex> guard(lock);
                inserted.push_back(
                    WorkloadInfo(inserted_user, to_insert.password));
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
    return inserted;
}

void do_auth(Http::Client& client, std::vector<WorkloadInfo>& credentials) {
    std::vector<Async::Promise<Http::Response>> responses;

    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/auth?";
    for (auto i = 0; i < credentials.size(); i++) {
        const auto& cred = credentials[i];
        auto posturl = url + "username=" + cred.user.username +
                       "&password=" + cred.password;
        auto response = client.post(posturl).send();
        response.then(
            [i, &credentials](Http::Response rsp) {
                ASSERT_EQ(rsp.code(), Http::Code::Ok);
                ASSERT_TRUE(rsp.cookies().has("session"));
                credentials[i].session = rsp.cookies().get("session");
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

void update_users(Http::Client& client,
                  std::vector<WorkloadInfo>& credentials) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/update?";

    for (auto& cred : credentials) {
        auto puturl = url + "id=" + std::to_string(cred.user.id);
        if (rand() % 2) {
            cred.user.name = fake::user::name();
            puturl += "&name=" + cred.user.name;
        }
        auto response = client.put(puturl).cookie(cred.session).send();
        response.then(
            [&](Http::Response rsp) { ASSERT_EQ(rsp.code(), Http::Code::Ok); },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

void select_users(Http::Client& client,
                  const std::vector<WorkloadInfo>& credentials,
                  bool ASSERT_fail) {
    std::vector<Async::Promise<Http::Response>> responses;

    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/show?";

    for (auto& cred : credentials) {
        auto geturl = url + "id=" + std::to_string(cred.user.id);
        auto response = client.get(geturl).send();
        response.then(
            [&cred, &ASSERT_fail](Http::Response rsp) {
                if (ASSERT_fail)
                    ASSERT_EQ(rsp.code(), Http::Code::Bad_Request);
                else {
                    ASSERT_EQ(rsp.code(), Http::Code::Ok);
                    auto user =
                        serialization::from_json<model::User>(rsp.body());
                    ASSERT_EQ(user, cred.user);
                }
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}

void drop_users(Http::Client& client,
                const std::vector<WorkloadInfo>& credentials,
                bool ASSERT_fail) {
    std::vector<Async::Promise<Http::Response>> responses;
    const std::string url =
        "http://" + ADDRESS + ":" + std::to_string(PORT) + "/0.0/users/drop?";

    for (auto& cred : credentials) {
        auto delurl = url + "id=" + std::to_string(cred.user.id);
        auto response = client.del(delurl).cookie(cred.session).send();
        response.then(
            [&](Http::Response rsp) {
                if (ASSERT_fail)
                    ASSERT_EQ(rsp.code(), Http::Code::Bad_Request);
                else
                    ASSERT_EQ(rsp.code(), Http::Code::Ok);
            },
            onfail);
        responses.push_back(std::move(response));
    }
    awaitall(responses);
}
