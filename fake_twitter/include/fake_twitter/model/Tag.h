#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter::model {

struct Tag {
    const PKey id;
    std::string title;
};

}  // namespace fake_twitter::model