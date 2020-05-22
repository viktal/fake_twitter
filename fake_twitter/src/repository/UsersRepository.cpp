#include "fake_twitter/repository/UsersRepository.h"

using namespace fake_twitter;
using namespace fake_twitter::repository;
using fake_twitter::repository::UsersRepository;

UsersRepository::UsersRepository(std::shared_ptr<DBConnectionsPool> pool) {
    this->pool = std::move(pool);
}

std::unique_ptr<model::User> UsersRepository::get(PKey id) {
    auto query =
        select(all_of(tabUsers)).from(tabUsers).where(tabUsers.id == id);

    auto result = pool->run(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::User> user;
    user = std::make_unique<model::User>(
        model::User{first.id.value(), first.name.value(),
                    first.username.value(), first.password_hash.value(),
                    first.avatar.value(), (size_t)first.followers_count.value(),
                    (size_t)first.friends_count.value()});

    return user;
}

model::User UsersRepository::create(const std::string& name,
                                    const std::string& username) {
    auto newid = pool->run(
        sqlpp::postgresql::insert_into(tabUsers)
            .set(tabUsers.name = name, tabUsers.username = username,
                 tabUsers.password_hash = 123, tabUsers.friends_count = 0,
                 tabUsers.followers_count = 0, tabUsers.avatar = "path")
            .returning(tabUsers.id));

    return std::move(model::User{PKey(newid.front().id.value()), name, username,
                                 123, "path", 0, 0});
}

bool UsersRepository::drop(PKey id) {
    return pool->run(remove_from(tabUsers).where(tabUsers.id == id));
}

void UsersRepository::update(PKey id, std::optional<std::string> name,
                             std::optional<std::string> avatar) {
    if (!name && !avatar) return;

    // workaround to make dynamic update
    auto query = sqlpp::blank_update_t<sqlpp::postgresql::connection>()
                     .single_table(tabUsers)
                     .dynamic_set();
    if (name) query.assignments.add(tabUsers.name = name.value());
    if (avatar) query.assignments.add(tabUsers.avatar = avatar.value());

    pool->run(query.where(tabUsers.id == id));
}

std::unique_ptr<model::Followers> UsersRepository::getfollow(PKey id) {
    auto query = select(all_of(tabFollower))
                     .from(tabFollower)
                     .where(tabFollower.id == id);

    auto result = pool->run(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::Followers> follower;
    follower = std::make_unique<model::Followers>(model::Followers{
        first.id.value(), first.author.value(), first.addresser.value()});

    return follower;
}

bool UsersRepository::follow(PKey author, PKey addresser) {
    auto result = pool->run(select(all_of(tabFollower))
                                .from(tabFollower)
                                .where(tabFollower.author == author &&
                                       tabFollower.addresser == addresser));
    if (result.empty()) {
        auto dummy2 = pool->run(
            sqlpp::update(tabUsers)
                .set(tabUsers.followers_count = tabUsers.followers_count + 1)
                .where(tabUsers.id == addresser));
        if (!dummy2) {
            return false;
        }
        dummy2 = pool->run(
            sqlpp::update(tabUsers)
                .set(tabUsers.friends_count = tabUsers.friends_count + 1)
                .where(tabUsers.id == author));

        if (!dummy2) {
            auto dummy = pool->run(sqlpp::update(tabUsers)
                                       .set(tabUsers.followers_count =
                                                tabUsers.followers_count - 1)
                                       .where(tabUsers.id == addresser));
            return false;
        }
        auto dummy = pool->run(insert_into(tabFollower)
                                   .set(tabFollower.author = author,
                                        tabFollower.addresser = addresser));
        return true;
    }
    return false;
}

bool UsersRepository::unfollow(PKey author, PKey addresser) {
    auto result = pool->run(remove_from(tabFollower)
                                .where(tabFollower.author == author &&
                                       tabFollower.addresser == addresser));
    if (!result) return false;
    auto dummy = pool->run(
        sqlpp::update(tabUsers)
            .set(tabUsers.followers_count = tabUsers.followers_count - 1)
            .where(tabUsers.id == addresser));

    dummy =
        pool->run(sqlpp::update(tabUsers)
                      .set(tabUsers.friends_count = tabUsers.friends_count - 1)
                      .where(tabUsers.id == author));
    return true;
}
