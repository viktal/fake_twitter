#pragma once

#include <mutex>
#include <sqlpp11/sqlpp11.h>

namespace fake_twitter::repository {

// TODO: Более разумная многопоточность, сейчас только 1 поток и все
// TODO: Шаблонный класс, относительнло sqlpp connection
template<typename ConnectionT>
class DBConnectionsPool {
public:
    explicit DBConnectionsPool(std::unique_ptr<ConnectionT> connection)  {
        this->connection = std::move(connection);
    }

    template <typename T>
    auto run(const T& query)
    {
        std::lock_guard<std::mutex> guard(lock);
        return (*connection)(query);
    }

private:
    std::mutex lock;
    std::unique_ptr<ConnectionT> connection;
};
}
