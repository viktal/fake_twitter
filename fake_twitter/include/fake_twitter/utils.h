#pragma once

#include <date/date.h>

#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>

#include "fake_twitter/common.h"

namespace fake_twitter::utils {

struct Session {
    const PKey user_id;
};

std::string salt();
PasswordHash make_password_hash(const std::string& salt,
                                const std::string& password);

}  // namespace fake_twitter::utils
