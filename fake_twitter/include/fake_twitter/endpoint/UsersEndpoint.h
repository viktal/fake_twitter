#pragma once

#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

#include <optional>

#include "fake_twitter/repository/UsersRepository.h"
#include "fake_twitter/serializer/json.h"
#include "fake_twitter/utils.h"

namespace fake_twitter::endpoints {

class UsersEndpoint {
public:
    explicit UsersEndpoint(
        std::shared_ptr<repository::UsersRepository> usersRepository) {
        this->usersRepository = std::move(usersRepository);
    };

    void show(const Pistache::Rest::Request& request,
              Pistache::Http::ResponseWriter response);

    void create(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    void update(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    void drop(const Pistache::Rest::Request& request,
              Pistache::Http::ResponseWriter response);

    void showFollowTable(const Pistache::Rest::Request& request,
                         Pistache::Http::ResponseWriter response);

    void follow(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    void unfollow(const Pistache::Rest::Request& request,
                  Pistache::Http::ResponseWriter response);

    void authorization(const Pistache::Rest::Request& request,
                       Pistache::Http::ResponseWriter response);

private:
    std::shared_ptr<repository::UsersRepository> usersRepository;
};

}  // namespace fake_twitter::endpoints
