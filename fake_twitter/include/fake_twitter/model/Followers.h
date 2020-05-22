#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter::model {

struct Followers {
    const PKey id;
    PKey author;
    PKey addresser;
};

}  // namespace fake_twitter::model
