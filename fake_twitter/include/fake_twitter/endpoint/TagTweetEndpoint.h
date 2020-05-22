#pragma once

#include <fake_twitter/serializer/json.h>
#include <pistache/router.h>
#include <rapidjson/rapidjson.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/repository/TagTweetRepository.h"

namespace fake_twitter::endpoints {

class TagTweetEndpoint {
public:
    explicit TagTweetEndpoint(
        std::shared_ptr<repository::TagTweetRepository> tagtweetRepository) {
        this->tagtweetRepository = std::move(tagtweetRepository);
    };

    void show(const Pistache::Http::Request& request,
              Pistache::Http::ResponseWriter response);
    void create(const Pistache::Http::Request& request,
                Pistache::Http::ResponseWriter response);

private:
    std::shared_ptr<repository::TagTweetRepository> tagtweetRepository;
};

}  // namespace fake_twitter::endpoints