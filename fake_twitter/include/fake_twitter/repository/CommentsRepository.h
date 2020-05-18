#pragma once

#include <sqlpp11/sqlpp11.h>
#include <optional>
#include "fake_twitter/model/Comment.h"
#include "fake_twitter/sqlpp_models/CommentsTab.h"
#include "fake_twitter/repository/DBConnectionsPool.h"

#include <memory>
#include <utility>


namespace fake_twitter::repository {
    class CommentsRepository {
    public:
        explicit CommentsRepository(std::shared_ptr<DBConnectionsPool> pool)
        {
            this->pool = std::move(pool);
        }
        virtual ~CommentsRepository() = default;
        virtual std::unique_ptr<model::Comment> get(PKey id);
        virtual std::unique_ptr<model::Comment> create(const std::string& body, const int& author, const int& comment_for);
        virtual bool Delete(PKey id);
        virtual std::unique_ptr<model::Comment> RaseLikes(PKey id);
    private:
        std::shared_ptr<DBConnectionsPool> pool;
        fake_twitter::sqlpp_models::TabComments tab;
    };



    std::unique_ptr<model::Comment> CommentsRepository::get(PKey id) {
        auto query = (select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        auto result = pool->run(query);
        if (result.empty()) {
            return nullptr;
        }

        auto &first = result.front();
        std::unique_ptr<model::Comment> comment;
        comment = std::make_unique<model::Comment>(first.id.value(), first.body.value(), first.author.value(),
                               first.comment_for.value(), first.rating.value());
        return comment;
    }

    std::unique_ptr<model::Comment> CommentsRepository::create(const std::string& body, const int& author, const int& comment_for) {
        auto newid = pool->run(insert_into(tab).set(
                tab.body = body,
                tab.author = author,
                tab.comment_for = comment_for,
                tab.rating = 0,
                tab.create_date = std::chrono::system_clock::now()));
        std::unique_ptr<model::Comment> comment;
        comment = std::make_unique<model::Comment>(PKey(newid), body, author,
                                                   comment_for, 0);
        return comment;
    }

    bool CommentsRepository::Delete(PKey id) {
        return pool->run(remove_from(tab).where(tab.id == id));
    }

    std::unique_ptr<model::Comment> CommentsRepository::RaseLikes(PKey id) {
        auto q = pool->run(update(tab).set(tab.rating = tab.rating + 1).where(tab.id == id));
        auto query = (select(all_of(tab)).from(tab)
                .where(tab.id == id));
        auto result = pool->run(query);
        if (result.empty()) {
            return nullptr;
        }

        auto &first = result.front();
        std::unique_ptr<model::Comment> comment;
        comment = std::make_unique<model::Comment>(first.id.value(), first.body.value(), first.author.value(),
                                                   first.comment_for.value(), first.rating.value());
        return comment;
    }


} //end namespace fake_twitter::repository