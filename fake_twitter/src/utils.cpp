#include "fake_twitter/utils.h"

using namespace fake_twitter::utils;

std::string fake_twitter::utils::salt() {
    auto now = std::chrono::system_clock::now();
    return date::format("%FT%TZ", now);
}

fake_twitter::PasswordHash fake_twitter::utils::make_password_hash(
    const std::string& salt, const std::string& password) {
    auto tohash = salt + password;
    long res = 0;
    int p = 31;
    for (int a : tohash) res = ((res + a - 64) * p) % LONG_MAX;
    return res;
}
