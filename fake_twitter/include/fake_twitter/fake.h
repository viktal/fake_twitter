#pragma once
#include "fake_twitter/model/User.h"

namespace fake_twitter::fake{
    model::User user () {
        return model::User{0, "user", "user", 123};
    }
}
