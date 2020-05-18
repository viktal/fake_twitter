#pragma once
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

#include "fake_twitter/model/Comment.h"
#include "fake_twitter/serializer/json.h"
#include "fake_twitter/repository/CommentsRepository.h"

namespace fake_twitter::endpoints {
    class CommentsEndpoint {
    public:
        explicit CommentsEndpoint(std::shared_ptr<repository::CommentsRepository> CommentsRepository) {
            this->CommentsRepository = std::move(CommentsRepository);
        };

        void show(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void Delete(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void showCommentsForTweet(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void RaseLikes(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    private:
        std::shared_ptr<repository::CommentsRepository> CommentsRepository;
    };

    void CommentsEndpoint::show(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabComments;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());

        std::unique_ptr<model::Comment> comment = CommentsRepository->get(id);
        if (!comment) {
            response.send(Pistache::Http::Code::Bad_Request, "No user with this id");
            return;
        }

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(*comment));
    }

    void CommentsEndpoint::create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabComments;
        auto author_optional = request.query().get("author");
        auto comment_for_optional = request.query().get("comment_for");
        auto body_json = request.body();
        if (author_optional.isEmpty() ||
            comment_for_optional.isEmpty() ||
            body_json.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "Not found one or more parameters");
            return;
        }
        auto author = std::stol(author_optional.get());
        auto comment_for = std::stol(comment_for_optional.get());
        Document document;
        document.Parse(body_json.c_str());
        auto body = std::string(document["body"].GetString());
        std::unique_ptr<model::Comment> comment = CommentsRepository->create(body, author, comment_for);

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(*comment));
    }

    void CommentsEndpoint::Delete(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using namespace rapidjson;
        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = stol(id_optional.get());
        bool deleted = CommentsRepository->Delete(id);
        if(deleted)
            response.send(Pistache::Http::Code::Ok, "Comment deleted");
        else
            response.send(Pistache::Http::Code::Bad_Request, "Comment not deleted");
    }

    void CommentsEndpoint::RaseLikes(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabComments;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());

        std::unique_ptr<model::Comment> comment = CommentsRepository->RaseLikes(id);
        if (!comment) {
            response.send(Pistache::Http::Code::Bad_Request, "No user with this id");
            return;
        }

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(*comment));
    }
   /* void CommentsEndpoint::showCommentsForTweet(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using namespace rapidjson;

        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;

        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());
        TabComments tab;
        model::Comment *comment = new model::Comment[6];
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.comment_for == id));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No comment with this id");
            delete[]comment;
            return;
        }
        int i;
        while (!result.empty()) {
            const auto &row = result.front();
            comment[i].set(row.id.value(), row.body.value(), row.author.value(), row.comment_for.value());
            i++;
            result.pop_front();
        }
        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(comment, i));
        delete[]comment;
        return;
    }*/

}

    /*void  showCommentsFotTweet(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using namespace rapidjson;

        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;

        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());
        TabComments tab;
        //auto result = (*db)(select(all_of(tab)).from(tab)
        //.where(tab.comment_for == id));
        model::Comment *comment = new model::Comment[6];
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.comment_for == id));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No comment with this id");
            delete[]comment;
            return;
        }
        int i;
        while (!result.empty()) {
            const auto &row = result.front();
            comment[i].set(row.id.value(), row.body.value(), row.author.value(), row.comment_for.value());
            i++;
            result.pop_front();
        }
        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(comment, i));
        delete[]comment;
        return;
    }*/


