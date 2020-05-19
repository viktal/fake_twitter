#pragma once

#include <gmock/gmock.h>

#include "fake_twitter/repository/UsersRepository.h"

class MockUsersRepository : public fake_twitter::repository::UsersRepository {
public:
    MockUsersRepository()
        : fake_twitter::repository::UsersRepository(nullptr) {}
    MOCK_METHOD(std::unique_ptr<fake_twitter::model::User>, get,
                (fake_twitter::PKey id), (override));
    MOCK_METHOD(fake_twitter::model::User, create,
                (const std::string& name, const std::string& username),
                (override));
    MOCK_METHOD(bool, drop, (fake_twitter::PKey id), (override));
    MOCK_METHOD(void, update,
                (fake_twitter::PKey id, std::optional<std::string> name,
                 std::optional<std::string> avatar),
                (override));
    MOCK_METHOD(std::unique_ptr<fake_twitter::model::Followers>, getfollow,
                (fake_twitter::PKey id), (override));
    MOCK_METHOD(bool, follow,
                (fake_twitter::PKey author, fake_twitter::PKey addresser),
                (override));
    MOCK_METHOD(bool, unfollow,
                (fake_twitter::PKey author, fake_twitter::PKey addresser),
                (override));
};
