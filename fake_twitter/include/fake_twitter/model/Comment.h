#pragma once

#include "fake_twitter/common.h"
//#include "fake_twitter/model/User.h"

namespace fake_twitter::model {

struct Comment {
    Comment(PKey id_t, std::string body_t, PKey author_t, PKey comment_for_t,
            int rating_t)
        : id(id_t),
          body(body_t),
          author(author_t),
          comment_for(comment_for_t),
          rating(rating_t){};
    Comment()
        : id(0),
          body("void"),
          author(0),
          comment_for(0),
          create_date("12:12:12"),
          rating(0){};
    void set(PKey id_t, std::string body_t, PKey author_t, PKey comment_for_t) {
        id = id_t;
        body = body_t;
        author = author_t;
        comment_for = comment_for_t;
    };
    PKey id;
    std::string body;
    PKey author;
    PKey comment_for;
    std::string create_date = "12:12:12";
    size_t rating;
};

}  // namespace fake_twitter::model