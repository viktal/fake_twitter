#pragma once
#include <pistache/router.h>
#include <rapidjson/rapidjson.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/model/Comment.h"
#include "fake_twitter/repository/CommentsRepository.h"
#include "fake_twitter/serializer/json.h"

namespace fake_twitter::endpoints {
class CommentsEndpoint {
public:
    explicit CommentsEndpoint(
        std::shared_ptr<repository::CommentsRepository> CommentsRepository) {
        this->CommentsRepository = std::move(CommentsRepository);
    };

    void show(const Pistache::Rest::Request& request,
              Pistache::Http::ResponseWriter response);

    void create(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    void Delete(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    void showCommentsForTweet(const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response);

    void RaseLikes(const Pistache::Rest::Request& request,
                   Pistache::Http::ResponseWriter response);

private:
    std::shared_ptr<repository::CommentsRepository> CommentsRepository;
};
}  // namespace fake_twitter::endpoints
