#include "fake_twitter/repository/CommentsRepository.h"

using namespace fake_twitter;
using namespace fake_twitter::repository;

std::unique_ptr<model::Comment> CommentsRepository::get(PKey id) {
    auto query = (select(all_of(tab)).from(tab).where(tab.id == id));
    auto result = pool->get_connection()(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::Comment> comment;
    comment = std::make_unique<model::Comment>(
        first.id.value(), first.body.value(), first.author.value(),
        first.comment_for.value(), first.rating.value(), std::chrono::time_point_cast<std::chrono::seconds>(
                    first.create_date.value()));
    return comment;
}

std::unique_ptr<model::Comment> CommentsRepository::create(
    const std::string& body, const int& author, const int& comment_for) {
    auto time = std::chrono::time_point_cast<std::chrono::seconds>(
            std::chrono::system_clock::now());
    auto newid = pool->get_connection()(insert_into(tab).set(
        tab.body = body, tab.author = author, tab.comment_for = comment_for,
        tab.rating = 0, tab.create_date = time));
    std::unique_ptr<model::Comment> comment;
    comment = std::make_unique<model::Comment>(PKey(newid), body, author,
                                               comment_for, 0, time);
    return comment;
}

bool CommentsRepository::Delete(PKey id) {
    return pool->get_connection()(remove_from(tab).where(tab.id == id));
}

std::unique_ptr<model::Comment> CommentsRepository::RaseLikes(PKey id) {
    auto q = pool->get_connection()(
        update(tab).set(tab.rating = tab.rating + 1).where(tab.id == id));
    auto query = (select(all_of(tab)).from(tab).where(tab.id == id));
    auto result = pool->get_connection()(query);
    if (result.empty()) {
        return nullptr;
    }

    auto& first = result.front();
    std::unique_ptr<model::Comment> comment;
    comment = std::make_unique<model::Comment>(
        first.id.value(), first.body.value(), first.author.value(),
        first.comment_for.value(), first.rating.value(), std::chrono::time_point_cast<std::chrono::seconds>(
                    first.create_date.value()));
    return comment;
}

std::vector<model::Comment> CommentsRepository::CommentsForTweet(PKey id) {
    std::vector<model::Comment> comments;
    auto queryComment =
            select(all_of(tab)).from(tab).where(tab.comment_for == id);
    auto resultComment = pool->get_connection()(queryComment);
    while (!resultComment.empty()) {
        auto& firstComment = resultComment.front();
        model::Comment c(firstComment.id.value(), firstComment.body.value(),
                          firstComment.author.value(), firstComment.comment_for.value(), firstComment.rating.value(), std::chrono::time_point_cast<std::chrono::seconds>(
                        firstComment.create_date.value()));
        comments.push_back(c);
        resultComment.pop_front();
    }
    return comments;
}