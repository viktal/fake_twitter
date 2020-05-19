#pragma once
#include <fake_twitter/serializer/json.h>
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/repository/NewsFeedRepository.h"

namespace fake_twitter::endpoints {
class NewsFeedEndpoint {
public:
    explicit NewsFeedEndpoint(
        std::shared_ptr<repository::NewsFeedRepository> newsFeedRepository) {
        this->newsFeedRepository = std::move(newsFeedRepository);
    };

    void show(const Pistache::Http::Request& request,
              Pistache::Http::ResponseWriter response);

private:
    std::shared_ptr<repository::NewsFeedRepository> newsFeedRepository;
};

}  // namespace fake_twitter::endpoints