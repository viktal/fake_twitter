#include "fake_twitter/model/User.h"

using namespace fake_twitter::model;

bool fake_twitter::model::operator==(const User& u1, const User& u2) {
    return u1.id == u2.id && u1.name == u2.name && u1.username == u2.username &&
           u1.password_hash == u2.password_hash && u1.salt == u2.salt &&
           u1.followers_count == u2.followers_count &&
           u1.friends_count == u2.friends_count;
}
