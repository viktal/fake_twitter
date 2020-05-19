#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter::model {

struct User {
    const PKey id;
    std::string name;
    std::string username;
    PasswordHash password_hash;
    Avatar avatar = "/path/";
    size_t followers_count = 0;
    size_t friends_count = 0;
};

bool operator==(const User& u1, const User& u2);

}  // namespace fake_twitter::model
