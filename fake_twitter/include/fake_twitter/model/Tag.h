#pragma once

#include "fake_twitter/common.h"


// TODO: create date
namespace fake_twitter::model {
    
struct Tag {
    const PKey id;
    std::string name;
    size_t rating = 0;
};

}