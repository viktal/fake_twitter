#pragma once

#include "fake_twitter/common.h"

// TODO: create date
namespace fake_twitter::model {

struct Tweet {
    const PKey id;
    std::string body;
    PKey author;
    std::string create_date = "12.12.12";
    size_t rating = 0;
    size_t retweets = 0;
};

}  // namespace fake_twitter::model