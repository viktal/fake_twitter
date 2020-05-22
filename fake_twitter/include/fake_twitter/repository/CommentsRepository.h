#pragma once

#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <optional>
#include <utility>

#include "fake_twitter/model/Comment.h"
#include "fake_twitter/repository/DBConnectionsPool.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"

namespace fake_twitter::repository {

class CommentsRepository {
public:
    explicit CommentsRepository(std::shared_ptr<DBConnectionsPool> pool) {
        this->pool = std::move(pool);
    }
    virtual ~CommentsRepository() = default;
    virtual std::unique_ptr<model::Comment> get(PKey id);
    virtual std::unique_ptr<model::Comment> create(const std::string& body,
                                                   const int& author,
                                                   const int& comment_for);
    virtual bool Delete(PKey id);
    virtual std::unique_ptr<model::Comment> RaseLikes(PKey id);

private:
    std::shared_ptr<DBConnectionsPool> pool;
    fake_twitter::sqlpp_models::TabComments tab;
};

}  // end namespace fake_twitter::repository