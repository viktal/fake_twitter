#pragma once

#include "fake_twitter/common.h"
//#include "fake_twitter/model/User.h"

namespace fake_twitter::model {

    struct Comment {
        const PKey id;
        std::string body;
        PKey author;
        PKey comment_for;
        std::string create_date = "12.12.12";
        size_t rating = 0;
    };

} // fake_twitter