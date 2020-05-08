#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter {
namespace model {

    struct User {
        const long id;
        std::string name;
        const std::string username;
        PasswordHash password_hash;
        Avatar avatar = "/path/";
        size_t followers_count = 0;
        size_t friends_count = 0;
    };

} // model
} // fake_twitter
