#include <pistache/client.h>

#include <filesystem>

#include "gtest/gtest.h"
#include "tweets_crud_test.h"
#include "users_crud_test.h"

using namespace Pistache;
using namespace fake_twitter;

TEST_F(test_fixture_restserver, test_many_users_create_show) {
    const int N = 20;

    auto credentials = make_users(*client, N);
    do_auth(*client, credentials);

    make_tweets(*client, credentials, N);
    select_tweets(*client, credentials, false);
//    drop_tweets(*client, credentials, false);
//
//    // can't select after drop or drop twice
//    select_tweets(*client, tweets, true);
//    drop_tweets(*client, tweets, true);

//    update_users(*client, credentials);
//    select_users(*client, credentials, false);
//    drop_users(*client, credentials, false);

//    // can't select after drop or drop twice
//    select_users(*client, credentials, true);
//    drop_users(*client, credentials, true);
}