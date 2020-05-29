#include <pistache/client.h>

#include <filesystem>

#include "gtest/gtest.h"
#include "tweets_crud_test.h"
#include "users_crud_test.h"

using namespace Pistache;
using namespace fake_twitter;

TEST_F(test_fixture_restserver, test_many_users_create_show) {
    const int users = 200;
    const int per_user_max_tweets = 20;

    auto credentials = make_users(*client, users);
    do_auth(*client, credentials);

//    make_tweets(*client, credentials, per_user_max_tweets);
//    select_tweets(*client, credentials, false);
//    drop_tweets(*client, credentials, false);
//
////     can't select after drop or drop twice
//    select_tweets(*client, credentials, true);
//    drop_tweets(*client, credentials, true);
//
//    update_users(*client, credentials);
//    select_users(*client, credentials, false);
//    drop_users(*client, credentials, false);
//
////     can't select after drop or drop twice
//    select_users(*client, credentials, true);
//    drop_users(*client, credentials, true);
}