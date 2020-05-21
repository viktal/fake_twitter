#include "fake_twitter/serializer/json.h"

using namespace rapidjson;
using namespace fake_twitter;

std::string serialization::to_json(model::User user) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember("id", user.id, allocator);
    d.AddMember("name", Value().SetString(StringRef(user.name.c_str())),
                allocator);
    d.AddMember("username", Value().SetString(StringRef(user.username.c_str())),
                allocator);
    d.AddMember("followers_count", user.followers_count, allocator);
    d.AddMember("friends_count", user.friends_count, allocator);
    d.AddMember("avatar", Value().SetString(StringRef(user.avatar.c_str())),
                allocator);
    d.AddMember("password_hash", user.password_hash, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string serialization::to_json(model::Tweet tweet) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember("id", tweet.id, allocator);
    d.AddMember("body", Value().SetString(StringRef(tweet.body.c_str())),
                allocator);
    d.AddMember("author", tweet.author, allocator);
    d.AddMember("create_date",
                Value().SetString(StringRef(tweet.create_date.c_str())),
                allocator);
    d.AddMember("retweets", tweet.retweets, allocator);
    d.AddMember("rating", tweet.rating, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string serialization::to_json(model::Followers followers) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember("id", followers.id, allocator);
    d.AddMember("author", followers.author, allocator);
    d.AddMember("addresser", followers.addresser, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string serialization::to_json(std::vector<model::Tweet> tweetVector) {
    std::string str = "{\"tweets\" :[";  // + "," + "]}";
    while (!tweetVector.empty()) {
        str = str + serialization::to_json(tweetVector.back());
        if (tweetVector.size() > 1) {
            str = str + ",";
        }
        tweetVector.pop_back();
    }
    str = str + "]}";
    return str;
}

std::string serialization::to_json(model::Comment comment) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember("id", comment.id, allocator);
    d.AddMember("body", Value().SetString(StringRef(comment.body.c_str())),
                allocator);
    d.AddMember("author", comment.author, allocator);
    d.AddMember("create_date",
                Value().SetString(StringRef(comment.create_date.c_str())),
                allocator);
    d.AddMember("comment_for", comment.comment_for, allocator);
    d.AddMember("rating", comment.rating, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string serialization::to_json(model::Comment* comment, int number) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    char table[6][32] = {"id",          "body",        "author",
                         "create_date", "comment_for", "rating"};
    for (int i = 0; i <= number; i++) {
        for (int j = 0; j < 6; j++) {
            char c[16];
            sprintf(c, "%d", i + 1);
            strcat(table[j], c);
        }
        d.AddMember(Value().SetString(StringRef(table[0])), comment[i].id,
                    allocator);
        d.AddMember(Value().SetString(StringRef(table[1])),
                    Value().SetString(StringRef(comment[i].body.c_str())),
                    allocator);
        d.AddMember(Value().SetString(StringRef(table[2])), comment[i].author,
                    allocator);
        d.AddMember(
            Value().SetString(StringRef(table[3])),
            Value().SetString(StringRef(comment[i].create_date.c_str())),
            allocator);
        d.AddMember(Value().SetString(StringRef(table[4])),
                    comment[i].comment_for, allocator);
        d.AddMember(Value().SetString(StringRef(table[5])), comment[i].rating,
                    allocator);
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

template <>
model::User serialization::from_json<model::User>(const std::string& json) {
    Document document;
    document.Parse(json.c_str());
    return model::User{document["id"].Get<PKey>(),
                       document["name"].GetString(),
                       document["username"].GetString(),
                       document["password_hash"].Get<PasswordHash>(),
                       document["avatar"].GetString(),
                       document["followers_count"].Get<size_t>(),
                       document["friends_count"].Get<size_t>()};
}

template <>
model::Tweet serialization::from_json<model::Tweet>(const std::string& json) {
    Document document;
    document.Parse(json.c_str());
    return model::Tweet{
        document["id"].Get<PKey>(),       document["body"].GetString(),
        document["author"].Get<PKey>(),   document["create_date"].GetString(),
        document["rating"].Get<size_t>(), document["retweets"].Get<size_t>()};
}
