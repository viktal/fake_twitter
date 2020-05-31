#pragma once

#include <gmock/gmock.h>

#include "fake_twitter/repository/TweetsRepository.h"
#include "fake_twitter/repository/UsersRepository.h"

class MockUsersRepository : public fake_twitter::repository::UsersRepository {
public:
    MockUsersRepository()
        : fake_twitter::repository::UsersRepository(nullptr) {}
    MOCK_METHOD(std::unique_ptr<fake_twitter::model::User>, get,
                (fake_twitter::PKey id), (override));
    MOCK_METHOD(fake_twitter::model::User, create,
                (const std::string& name, const std::string& username,
                 const std::string& password),
                (override));
    MOCK_METHOD(bool, drop, (fake_twitter::PKey id), (override));
    MOCK_METHOD(void, update,
                (fake_twitter::PKey id, std::optional<std::string> name),
                (override));
    MOCK_METHOD(std::vector<fake_twitter::model::User_pr>, getfollow,
                (fake_twitter::PKey id), (override));
    MOCK_METHOD(bool, follow,
                (fake_twitter::PKey author, fake_twitter::PKey addresser),
                (override));
    MOCK_METHOD(bool, unfollow,
                (fake_twitter::PKey author, fake_twitter::PKey addresser),
                (override));
};

class MockTweetsRepository : public fake_twitter::repository::TweetsRepository {
public:
    MockTweetsRepository()
        : fake_twitter::repository::TweetsRepository(nullptr) {}

    MOCK_METHOD(std::unique_ptr<fake_twitter::model::Tweet>, get,
                (fake_twitter::PKey id), (override));
    MOCK_METHOD(fake_twitter::model::Tweet, create,
                (const fake_twitter::PKey& author, const std::string& body),
                (override));
    MOCK_METHOD(bool, drop, (fake_twitter::PKey id), (override));
    MOCK_METHOD(bool, like,
                (fake_twitter::PKey author, fake_twitter::PKey twit),
                (override));
    MOCK_METHOD(bool, unlike,
                (fake_twitter::PKey author, fake_twitter::PKey twit),
                (override));
};
