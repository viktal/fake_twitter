#pragma once

#include <fake_twitter/serializer/json.h>
#include <pistache/router.h>
#include <rapidjson/rapidjson.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/repository/TweetsRepository.h"

namespace fake_twitter::endpoints {

class TweetsEndpoint {
public:
    explicit TweetsEndpoint(
        std::shared_ptr<repository::TweetsRepository> tweetsRepository) {
        this->tweetsRepository = std::move(tweetsRepository);
    };

    void show(const Pistache::Http::Request& request,
              Pistache::Http::ResponseWriter response);
    void drop(const Pistache::Http::Request& request,
              Pistache::Http::ResponseWriter response);
    void create(const Pistache::Http::Request& request,
                Pistache::Http::ResponseWriter response);

    void like(const Pistache::Http::Request& request,
              Pistache::Http::ResponseWriter response);
    void unlike(const Pistache::Http::Request& request,
                Pistache::Http::ResponseWriter response);

private:
    std::shared_ptr<repository::TweetsRepository> tweetsRepository;
};

}  // namespace fake_twitter::endpoints
