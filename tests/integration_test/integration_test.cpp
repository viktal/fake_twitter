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

    auto workload = make_users(*client, users);
    do_auth(*client, workload);

    make_tweets(*client, workload, per_user_max_tweets);
    select_tweets(*client, workload, false);
    drop_tweets(*client, workload, false);

    //     can't select after drop or drop twice
    select_tweets(*client, workload, true);
    drop_tweets(*client, workload, true);

    update_users(*client, workload);
    select_users(*client, workload, false);
    drop_users(*client, workload, false);

    //     can't select after drop or drop twice
    select_users(*client, workload, true);
    drop_users(*client, workload, true);
}