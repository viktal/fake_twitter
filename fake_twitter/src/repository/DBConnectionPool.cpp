#include "fake_twitter/repository/DBConnectionsPool.h"

using namespace fake_twitter::repository;

DBConnectionsPool::DBConnectionsPool(
    std::unique_ptr<sqlpp::postgresql::connection> connection) {
    this->connection = std::move(connection);
}
