#pragma once

#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/sqlpp11.h>

#include <mutex>

namespace fake_twitter::repository {
// TODO: Более разумная многопоточность, сейчас только 1 поток и все
class DBConnectionsPool {
public:
    explicit DBConnectionsPool(
        std::unique_ptr<sqlpp::postgresql::connection> connection);

    template <typename T>
    auto run(const T& query) {
        std::lock_guard<std::mutex> guard(lock);
        return (*connection)(query);
    };

private:
    std::mutex lock;
    std::unique_ptr<sqlpp::postgresql::connection> connection;
};
}  // namespace fake_twitter::repository
