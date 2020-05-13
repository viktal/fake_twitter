#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <pistache/http.h>

#include "fake_twitter/repository/usersRepository.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"
#include "fake_twitter/model/User.h"
#include "fake_twitter/common.h"

#include "fake_twitter/serializer/json.h"
#include "fake_twitter/sqlpp_models/UsersTab.h"
#include "fake_twitter/repository/usersRepository.h"
#include "fake_twitter/endpoint/UsersEndpoint.h"

using namespace Pistache;
using namespace fake_twitter;
using namespace Rest;

class MockUsersRepository : public repository::UsersRepository {
public:
    MockUsersRepository() : UsersRepository(nullptr) {}
    MOCK_METHOD(std::unique_ptr<model::User>, get, (PKey id));
};

class MockResponseWriter {
public:
    MockResponseWriter() = default;
//    void setMime(const Mime::MediaType &mime);

//    Async::Promise<ssize_t>
//    sendMethodNotAllowed(const std::vector<Http::Method> &supportedMethods);


    MOCK_METHOD(Async::Promise<ssize_t>, send_impl, (Http::Code code, const std::string &body, const Http::Mime::MediaType &mime));
    Async::Promise<ssize_t> send(Http::Code code, const std::string &body = "",
                                 const Http::Mime::MediaType &mime = Http::Mime::MediaType()) {
        return send_impl(code, body, mime);
    }

//    template <size_t N>
//    Async::Promise<ssize_t>
//    send(Code code, const char (&arr)[N],
//         const Mime::MediaType &mime = Mime::MediaType()) {
//        return sendImpl(code, arr, N - 1, mime);
//    }
//
//    Async::Promise<ssize_t> send(Code code, const char *data, const size_t size,
//                                 const Mime::MediaType &mime = Mime::MediaType());

//    ResponseStream stream(Code code, size_t streamSize = DefaultStreamSize);

//    template <typename Duration> void timeoutAfter(Duration duration) {
//        timeout_.arm(duration);
//    }

//    CookieJar &cookies();
//
//    Header::Collection &headers();
//
//    Timeout &timeout();
//
//    std::shared_ptr<Tcp::Peer> peer() const;

//    ssize_t getResponseSize() const { return sent_bytes_; }

//   Code getResponseCode() const { return response_.code(); }

};

class MockRequest {
public:
    MOCK_METHOD(const Http::Uri::Query&, query, (), (const));
//    bool hasParam(const std::string &name) const;
//    TypedParam param(const std::string &name) const;
//    TypedParam splatAt(size_t index) const;
//    std::vector<TypedParam> splat() const;
//    const Address &address() const;
//    void copyAddress(const Address &address) { address_ = address; }
//    std::chrono::milliseconds timeout() const;
//    Version version() const;
//    Code code() const;
//    std::string body() const;
//    const CookieJar &cookies() const;
//    CookieJar &cookies();
//    const Header::Collection &headers() const;
//    Header::Collection &headers();
};

class test_fixture_for_client: public ::testing::Test {
public:

    void SetUp() override {

    }

    void TearDown() override{

    }
};

TEST_F(test_fixture_for_client, getByKey) {

    MockUsersRepository mockUsersRepository;
    std::shared_ptr<repository::UsersRepository> ptr;
    ptr.reset(&mockUsersRepository);

    EXPECT_CALL(mockUsersRepository, get)
            .Times(testing::AtLeast(1));

    MockResponseWriter mockResponseWriter;
    MockRequest mockRequest;
    auto usersEndpoint = endpoints::UsersEndpoint<MockRequest, MockResponseWriter>(ptr);

    usersEndpoint.show(mockRequest, mockResponseWriter);

}
