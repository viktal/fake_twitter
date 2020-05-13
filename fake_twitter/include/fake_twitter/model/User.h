#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter {
namespace model {

    struct User {
        const PKey id;
        std::string name;
        std::string username;
        PasswordHash password_hash;
        Avatar avatar = "/path/";
        size_t followers_count = 0;
        size_t friends_count = 0;
    };

    bool operator==(const User& u1, const User& u2) {
        return u1.id == u2.id &&
               u1.name == u2.name &&
               u1.username == u2.username &&
               u1.password_hash == u2.password_hash &&
               u1.avatar == u2.avatar &&
               u1.followers_count == u2.followers_count &&
               u1.friends_count == u2.friends_count;
    }

} // model
} // fake_twitter
