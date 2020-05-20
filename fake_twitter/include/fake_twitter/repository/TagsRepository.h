#pragma once

#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <optional>
#include <utility>

#include "fake_twitter/model/Tag.h"
#include "fake_twitter/repository/DBConnectionsPool.h"
#include "fake_twitter/sqlpp_models/TagsTab.h"

namespace fake_twitter::repository {
class TagsRepository {
public:
    explicit TagsRepository(std::shared_ptr<DBConnectionsPool> pool);

    virtual ~TagsRepository() = default;

    virtual std::unique_ptr<model::Tag> get(PKey id);
    virtual model::Tag create(const std::string& name);

private:
    std::shared_ptr<DBConnectionsPool> pool;
    fake_twitter::sqlpp_models::TabTags tabTags;
};

}  // end namespace fake_twitter::repository