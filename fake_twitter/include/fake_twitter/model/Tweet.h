#pragma once

#include "fake_twitter/common.h"

// TODO: create date
namespace fake_twitter::model {

struct Tweet {
    const PKey id;
    std::string body;
    PKey author;
    Date create_date;
    long rating = 0;
    long retweets = 0;
};

bool operator==(const Tweet& t1, const Tweet& t2);

}  // namespace fake_twitter::model