#include "fake_twitter/repository/TagsRepository.h"

using namespace fake_twitter;
using fake_twitter::repository::TagsRepository;

TagsRepository::TagsRepository(std::shared_ptr<DBConnectionsPool> pool) {
    this->pool = std::move(pool);
}

std::unique_ptr<model::Tag> TagsRepository::get(PKey id) {
    auto query = select(all_of(tabTags)).from(tabTags).where(tabTags.id == id);

    auto result = pool->get_connection()(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::Tag> tag;
    tag = std::make_unique<model::Tag>(
        model::Tag{first.id.value(), first.title.value()});

    return tag;
}

model::Tag TagsRepository::create(const std::string& title) {
    auto newid = pool->get_connection()(sqlpp::postgresql::insert_into(tabTags)
                                            .set(tabTags.title = title)
                                            .returning(tabTags.id));

    return std::move(model::Tag{PKey(newid.front().id.value()), title});
}