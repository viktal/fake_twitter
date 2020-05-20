#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter::model {

struct Twag {
    const PKey id;
    PKey twit;
    PKey tag;
};

}  // namespace fake_twitter::model