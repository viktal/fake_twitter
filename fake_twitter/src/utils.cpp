#include "fake_twitter/utils.h"

using namespace fake_twitter::utils;

// TODO:add MAC and IP address(or use uuid)
std::string fake_twitter::utils::salt() {
    auto now = std::chrono::system_clock::now();
    return date::format("%FT%TZ", now);
}

fake_twitter::PasswordHash fake_twitter::utils::make_password_hash(
    const std::string& salt, const std::string& password) {
    // TODO: proper hashing. std::hash is not consistent between runs
    //    return std::hash<std::string>{}(salt + password);
    auto tohash = salt + password;
    return std::accumulate(tohash.begin(), tohash.end(), 0);
}
