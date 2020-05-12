#pragma once
#include <pistache/router.h>
#include <sqlpp11/sqlpp11.h>

namespace fake_twitter::endpoints {
    class CommentsEndpoint {
    public:
        explicit CommentsEndpoint(std::unique_ptr<sqlpp::sqlite3::connection> db) {
            this->db = std::move(db);
        };

        void show(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void Delete(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void showCommentsForTweet(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        void RaseLikes(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    private:
        std::unique_ptr<sqlpp::sqlite3::connection> db;
    };

    void CommentsEndpoint::show(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabComments;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());

        TabComments tab;
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No comment with this id");
            return;
        }

        auto &first = result.front();
        model::Comment comment(first.id.value(), first.body.value(), first.author.value(),
                               first.comment_for.value(), first.rating.value());

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(comment));
    }

    void CommentsEndpoint::create(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using fake_twitter::sqlpp_models::TabComments;
        auto body_optional = request.query().get("body");
        auto author_optional = request.query().get("author");
        auto comment_for_optional = request.query().get("comment_for");
        if (body_optional.isEmpty() ||
            author_optional.isEmpty() ||
            comment_for_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "Not found one or more parameters");
            return;
        }
        auto body = std::string(body_optional.get());
        auto author = std::stol(author_optional.get());
        auto comment_for = std::stol(comment_for_optional.get());


        TabComments tab;
        (*db)(insert_into(tab).set(
                tab.body = body,
                tab.author = author,
                tab.comment_for = comment_for,
                tab.rating = 0,
                tab.create_date = std::chrono::system_clock::now()));


        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.body == body));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No comment with this username");
            return;
        }
        auto &first = result.front();
        model::Comment comment(first.id.value(), first.body.value(), first.author.value(),
                               first.comment_for.value(), first.rating.value());

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(comment));
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
        TabComments tab;
        (*db)(remove_from(tab).where(tab.id == id));

        response.send(Pistache::Http::Code::Ok, "Comment deleted");
    }

    void CommentsEndpoint::RaseLikes(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using namespace rapidjson;
        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;
        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = stol(id_optional.get());
        TabComments tab;
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.id == id));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No comment with this id");
            return;
        }
        (*db)(update(tab).set(tab.rating = tab.rating + 1).where(tab.id == id));
        result = (*db)(select(all_of(tab)).from(tab)
                               .where(tab.id == id));
        auto &first = result.front();
        model::Comment comment(first.id.value(), first.body.value(), first.author.value(),
                               first.comment_for.value(), first.rating.value());

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(comment));
    }
    void CommentsEndpoint::showCommentsForTweet(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
        using namespace rapidjson;

        using namespace fake_twitter;
        using fake_twitter::sqlpp_models::TabComments;

        auto id_optional = request.query().get("id");
        if (id_optional.isEmpty())
        {
            response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
            return;
        }
        auto id = std::stol(id_optional.get());
        TabComments tab;
        model::Comment* comment = new model::Comment[6];
        auto result = (*db)(select(all_of(tab)).from(tab)
                                    .where(tab.comment_for == id));
        if (result.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "No comment with this id");
            delete []comment;
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
        delete []comment;
        return;
    }

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


