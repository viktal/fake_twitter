#include <pistache/client.h>

#include <filesystem>

#include "gtest/gtest.h"
#include "tweets_crud_test.h"
#include "users_crud_test.h"

using namespace Pistache;
using namespace fake_twitter;

TEST_F(test_fixture_restserver, test_many_users_create_show) {
    const int N = 20;

    auto users = make_users(*client, N);

    auto tweets = make_tweets(*client, users, N);
    select_tweets(*client, tweets, false);
    drop_tweets(*client, tweets, false);

    // can't select after drop or drop twice
    select_tweets(*client, tweets, true);
    drop_tweets(*client, tweets, true);

    update_users(*client, users);
    select_users(*client, users, false);
    drop_users(*client, users, false);

    // can't select after drop or drop twice
    select_users(*client, users, true);
    drop_users(*client, users, true);
}