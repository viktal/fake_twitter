#include <pistache/client.h>

#include <filesystem>

#include "gtest/gtest.h"
#include "users_crud_test.h"

using namespace Pistache;
using namespace fake_twitter;

TEST_F(test_fixture_restserver, test_many_users_create_show) {
    const int N = 100;

    auto users = make_users(*client, N);

    //    auto tweets = make_tweets(*client, N);
    //    update_tweets(*client, users);
    //    select_tweets(*client, users, false);
    //    drop_tweets(*client, users, false);

    update_users(*client, users);
    select_users(*client, users, false);
    drop_users(*client, users, false);

    // can't select after drop or drop twice
    select_users(*client, users, true);
    drop_users(*client, users, true);
}