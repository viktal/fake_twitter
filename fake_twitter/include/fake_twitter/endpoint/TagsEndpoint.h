#pragma once

#include <fake_twitter/serializer/json.h>
#include <pistache/router.h>
#include <rapidjson/rapidjson.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/repository/TagsRepository.h"

namespace fake_twitter::endpoints {

class TagsEndpoint {
public:
    explicit TagsEndpoint(
        std::shared_ptr<repository::TagsRepository> tagsRepository) {
        this->tagsRepository = std::move(tagsRepository);
    };

    void show(const Pistache::Http::Request& request,
              Pistache::Http::ResponseWriter response);
    void create(const Pistache::Http::Request& request,
                Pistache::Http::ResponseWriter response);

private:
    std::shared_ptr<repository::TagsRepository> tagsRepository;
};

}  // namespace fake_twitter::endpoints