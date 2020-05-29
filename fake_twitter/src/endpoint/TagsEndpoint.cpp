#include "fake_twitter/endpoint/TagsEndpoint.h"

using namespace fake_twitter::endpoints;

void TagsEndpoint::show(const Pistache::Http::Request& request,
                        Pistache::Http::ResponseWriter response) {
    using fake_twitter::sqlpp_models::TabTags;
    auto id_optional = request.query().get("id");
    if (id_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request, "No id parameter");
        return;
    }
    auto id = std::stol(id_optional.get());
    std::unique_ptr<model::Tag> tag = tagsRepository->get(id);
    if (!tag) {
        response.send(Pistache::Http::Code::Bad_Request, "No tag with this id");
        return;
    } else {
        response.headers().add<Pistache::Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, serialization::to_json(*tag));
    }
}

void TagsEndpoint::create(const Pistache::Http::Request& request,
                          Pistache::Http::ResponseWriter response) {
    auto title_optional = request.query().get("title");
    if (title_optional.isEmpty()) {
        response.send(Pistache::Http::Code::Bad_Request,
                      "Not found one or more parameters");
        return;
    }
    auto title = std::string(title_optional.get());

    auto newTag = tagsRepository->create(title);

    response.headers().add<Pistache::Http::Header::ContentType>(
        MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, serialization::to_json(newTag));
}