#pragma once

#include <chrono>
#include <string>

namespace fake_twitter {

using PasswordHash = long;
using PKey = long;
using Date =
    std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;
using HTML = std::string;

}  // namespace fake_twitter
