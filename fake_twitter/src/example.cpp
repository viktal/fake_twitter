#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using namespace std;
using namespace Pistache;

class StatsEndpoint {
public:
    explicit StatsEndpoint(Address addr)
            : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
                .threads(static_cast<int>(thr));
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

    void setupRoutes() {
        using namespace Rest;
        Routes::Get(router, "/0.0/users/show.json", Routes::bind(&StatsEndpoint::handleReady, this));
    }

private:
    void handleReady(const Rest::Request &, Http::ResponseWriter response) {
        using namespace rapidjson;
//        const char *json = R"{
//                "id": 6253282,
//                "name": "Twitter API",
//                "username": "TwitterAPI",
//                "followers_count": 6133636,
//                "friends_count": 12,
//                "avatar": "/path/",
//                "password_hash": "hash"
//        }"
        Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

        d.AddMember("id", 6253282, d.GetAllocator());
        d.AddMember("name", "Twitter API", allocator);
        d.AddMember("username", "TwitterAPI", allocator);
        d.AddMember("followers_count", 6133636, allocator);
        d.AddMember("friends_count", 12, allocator);
        d.AddMember("avatar", "/path/", allocator);
        d.AddMember("password_hash", "hash", allocator);


//        d.Parse(json);

        // 2. Modify it by DOM.
//        Value &s = d["stars"];
//        s.SetInt(s.GetInt() + 1);

        // 3. Stringify the DOM
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Ok, buffer.GetString());

    }

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

};

int main(int argc, char *argv[]) {
    Port port(9081);

    int thr = 2;

    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3)
            thr = std::stoi(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    StatsEndpoint stats(addr);
    stats.init(thr);
    stats.start();
}
