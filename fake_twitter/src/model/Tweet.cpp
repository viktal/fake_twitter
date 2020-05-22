#include "fake_twitter/model/Tweet.h"

using namespace fake_twitter::model;

bool fake_twitter::model::operator==(const Tweet& t1, const Tweet& t2) {
    return t1.id == t2.id && t1.body == t2.body && t1.author == t2.author &&
           t1.create_date == t2.create_date && t1.rating == t2.rating &&
           t1.retweets == t2.retweets;
}
