/*
 * #include "gmock/gmock.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <pistache/client.h>
#include <pistache/endpoint.h>
#include "httplib.h"

#include "fake_twitter/repository/UsersRepository.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"
#include "fake_twitter/model/User.h"
#include "fake_twitter/common.h"

#include "fake_twitter/serializer/json.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/repository/UsersRepository.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"

using namespace Pistache;
using namespace fake_twitter;
namespace sql = sqlpp::sqlite3;

class MockUsersRepository : public repository::UsersRepository {
public:
    MockUsersRepository() : UsersRepository(nullptr) {
    }

//    MockUsersRepository(std::unique_ptr<sqlpp::sqlite3::connection> db) : UsersRepository(db) {}
    MOCK_METHOD(std::unique_ptr<model::User>, get, (PKey id));

};

class RestServer {
public:
    RestServer(Address addr, sql::connection_config config) {
        httpEndpoint = std::make_shared<Http::Endpoint>(addr);
        usersRepository = std::make_unique<repository::UsersRepository>
                (std::make_unique<sqlpp::sqlite3::connection>(config));
        usersEndpoint = std::make_shared<endpoints::UsersEndpoint>(usersRepository);
    }

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
                .threads(static_cast<int>(thr))
                .flags(Tcp::Options::ReusePort | Tcp::Options::ReuseAddr);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

    void setupRoutes() {
        using namespace Rest;

        Routes::Get(router,
                "/test/users/show.json",
                Routes::bind(&endpoints::UsersEndpoint::show,
                        usersEndpoint));
//        Routes::Post(router,
//                "/0.0/users/create",
//                Routes::bind(&endpoints::UsersEndpoint::create,
//                        usersEndpoint));
    }

private:
    std::shared_ptr<endpoints::UsersEndpoint>  usersEndpoint;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    std::shared_ptr<repository::UsersRepository> usersRepository;
    Rest::Router router;
};

class test_fixture_for_client: public ::testing::Test {
    void SetUp() override {
        Port port(9080);
        int thr = 1;
        Address addr(Ipv4::any(), port);

        std::cout << "Start server" << std::endl;
        sqlpp::sqlite3::connection_config config;

        config.path_to_database = "/tmp/db.sqlite_test";
        config.flags = SQLITE_OPEN_READWRITE;
        config.debug = true;
        RestServer stats(addr, config);
        stats.init(thr);
        stats.start();
    }

    void TearDown() override{

    }
};

TEST_F(test_fixture_for_client, getByKey) {
    MockUsersRepository mockUsersRepository;
    std::shared_ptr<repository::UsersRepository> ptr;
    ptr.reset(&mockUsersRepository);//= (repository::UsersRepository*);
    EXPECT_CALL(mockUsersRepository, get)
            .Times(testing::AtLeast(1));

    endpoints::UsersEndpoint usersEndpoint(ptr);

    Pistache::Http::Client client;
    auto client_opts = Pistache::Http::Client::options().threads(1).maxConnectionsPerHost(1);
    client.init(client_opts);

    auto response = client.post("url")
            .body("payload")
            .timeout(std::chrono::seconds(3))
            .send();

    const Pistache::Http::RequestParser parser(Pistache::Const::DefaultMaxRequestSize);

//    Http::Request request;
//    Http::Private::RequestLineStep step(&request);
    EXPECT_TRUE(usersEndpoint.show(response, const Rest::Request &) == nullptr);
}

/*
 * точка фиксации - статический, динамический полиморфизм
 * 1 - шаблоны....
 *      фиксация на этапе компиляции
 *      потребитель - шаблонный
 * 2 - добавить интерфейс с виртуальным методом и отнаследоваться
 *
 * */
*/