#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter::model {

struct TagTweet {
    const PKey id;
    PKey tweetID;
    PKey tagID;
};

}  // namespace fake_twitter::model
