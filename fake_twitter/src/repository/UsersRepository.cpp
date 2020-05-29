#include "fake_twitter/repository/UsersRepository.h"

#include <utility>

using namespace fake_twitter;
using namespace fake_twitter::repository;
using fake_twitter::repository::UsersRepository;

UsersRepository::UsersRepository(std::shared_ptr<DBConnectionsPool> pool) {
    this->pool = std::move(pool);
}

std::unique_ptr<model::User> UsersRepository::get(PKey id) {
    std::unique_ptr<model::User> user = get({id}, {});
    return user;
}

std::unique_ptr<model::User> repository::UsersRepository::get(
    const std::string& username) {
    //    std::optional<std::string> username_optional =
    //    std::optional<std::string>(username);
    std::unique_ptr<model::User> user = get({}, {username});
    return user;
}
std::unique_ptr<model::User> repository::UsersRepository::get(
    std::optional<PKey> id, std::optional<std::string> username) {
    auto query = sqlpp::blank_select_t<sqlpp::postgresql::connection>()
                     .columns(all_of(tabUsers))
                     .from(tabUsers)
                     .dynamic_where();

    if (id) query.where.add(tabUsers.id == id.value());
    if (username) query.where.add(tabUsers.username == username.value());

    auto result = pool->get_connection()(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::User> user;
    user = std::make_unique<model::User>(
        model::User{first.id.value(), first.name.value(),
                    first.username.value(), first.password_hash.value(),
                    first.salt.value(), (size_t)first.followers_count.value(),
                    (size_t)first.friends_count.value()});

    return user;
}

model::User UsersRepository::create(const std::string& name,
                                    const std::string& username,
                                    const std::string& password) {
    auto salt = utils::salt();
    auto password_hash = utils::make_password_hash(salt, password);

    auto newid = pool->get_connection()(
        sqlpp::postgresql::insert_into(tabUsers)
            .set(tabUsers.name = name, tabUsers.username = username,
                 tabUsers.password_hash = password_hash,
                 tabUsers.friends_count = 0, tabUsers.followers_count = 0,
                 tabUsers.salt = salt)
            .returning(tabUsers.id));

    return std::move(model::User{PKey(newid.front().id.value()), name, username,
                                 password_hash, salt, 0, 0});
}

bool UsersRepository::drop(PKey id) {
    return pool->get_connection()(
        remove_from(tabUsers).where(tabUsers.id == id));
}

void UsersRepository::update(PKey id, std::optional<std::string> name) {
    if (!name) return;

    // workaround to make dynamic update
    auto query = sqlpp::blank_update_t<sqlpp::postgresql::connection>()
                     .single_table(tabUsers)
                     .dynamic_set();
    if (name) query.assignments.add(tabUsers.name = name.value());

    pool->get_connection()(query.where(tabUsers.id == id));
}

std::unique_ptr<model::Followers> UsersRepository::getfollow(PKey id) {
    auto query = select(all_of(tabFollower))
                     .from(tabFollower)
                     .where(tabFollower.id == id);

    auto result = pool->get_connection()(query);
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
    auto result =
        pool->get_connection()(select(all_of(tabFollower))
                                   .from(tabFollower)
                                   .where(tabFollower.author == author &&
                                          tabFollower.addresser == addresser));
    if (result.empty()) {
        auto dummy2 = pool->get_connection()(
            sqlpp::update(tabUsers)
                .set(tabUsers.followers_count = tabUsers.followers_count + 1)
                .where(tabUsers.id == addresser));
        if (!dummy2) {
            return false;
        }
        dummy2 = pool->get_connection()(
            sqlpp::update(tabUsers)
                .set(tabUsers.friends_count = tabUsers.friends_count + 1)
                .where(tabUsers.id == author));

        if (!dummy2) {
            auto dummy = pool->get_connection()(
                sqlpp::update(tabUsers)
                    .set(tabUsers.followers_count =
                             tabUsers.followers_count - 1)
                    .where(tabUsers.id == addresser));
            return false;
        }
        auto dummy =
            pool->get_connection()(insert_into(tabFollower)
                                       .set(tabFollower.author = author,
                                            tabFollower.addresser = addresser));
        return true;
    }
    return false;
}

bool UsersRepository::unfollow(PKey author, PKey addresser) {
    auto result =
        pool->get_connection()(remove_from(tabFollower)
                                   .where(tabFollower.author == author &&
                                          tabFollower.addresser == addresser));
    if (!result) return false;
    auto dummy = pool->get_connection()(
        sqlpp::update(tabUsers)
            .set(tabUsers.followers_count = tabUsers.followers_count - 1)
            .where(tabUsers.id == addresser));

    dummy = pool->get_connection()(
        sqlpp::update(tabUsers)
            .set(tabUsers.friends_count = tabUsers.friends_count - 1)
            .where(tabUsers.id == author));
    return true;
}
