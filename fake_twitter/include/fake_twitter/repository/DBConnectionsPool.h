#pragma once

#include <sqlpp11/connection_pool.h>
#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/sqlpp11.h>

#include <mutex>

namespace fake_twitter::repository {

using DBConnectionsPool =
    sqlpp::connection_pool<sqlpp::postgresql::connection_config,
                           sqlpp::reconnect_policy::auto_reconnect,
                           sqlpp::postgresql::connection>;
}  // namespace fake_twitter::repository
