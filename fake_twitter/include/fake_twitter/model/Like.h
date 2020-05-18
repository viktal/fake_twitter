#pragma once

#include "fake_twitter/common.h"

namespace fake_twitter::model {

    struct Like {
        const PKey id;
        PKey author;
        PKey twit;
    };

} // fake_twitter
