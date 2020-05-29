#include "fake_twitter/serializer/json.h"

using namespace rapidjson;
using namespace fake_twitter;

std::string serialization::to_json(const model::User& user) {
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
    d.AddMember("password_hash", user.password_hash, allocator);
    d.AddMember("salt", Value().SetString(StringRef(user.salt.c_str())),
                allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string serialization::to_json(const utils::Session& session) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember("user_id", session.user_id, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string serialization::to_json(const model::Tweet& tweet) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    std::string time = date::format("%FT%TZ",
                                    date::floor<std::chrono::seconds>(tweet.create_date));

    d.AddMember("id", tweet.id, allocator);
    d.AddMember("body", Value().SetString(StringRef(tweet.body.c_str())),
                allocator);
    d.AddMember("author", tweet.author, allocator);
    d.AddMember("create_date", Value().SetString(StringRef(time.c_str())),
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
                       document["salt"].GetString(),
                       document["followers_count"].Get<size_t>(),
                       document["friends_count"].Get<size_t>()};
}

template <>
utils::Session serialization::from_json<utils::Session>(
    const std::string& json) {
    Document document;
    document.Parse(json.c_str());
    return utils::Session{document["user_id"].Get<PKey>()};
}


#include <iostream>
template <>
model::Tweet serialization::from_json<model::Tweet>(const std::string& json) {
    Document document;
    document.Parse(json.c_str());

    std::istringstream in{document["create_date"].GetString()};
    date::sys_time<std::chrono::seconds> tp;
    in >> date::parse("%FT%TZ", tp);

    return model::Tweet{
        document["id"].Get<PKey>(),       document["body"].GetString(),
        document["author"].Get<PKey>(),   tp,
        document["rating"].Get<long>(), document["retweets"].Get<long>()};
}

std::string serialization::to_json(model::Tag tag) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember("id", tag.id, allocator);
    d.AddMember("title", Value().SetString(StringRef(tag.title.c_str())),
                allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string serialization::to_json(model::TagTweet tagtweet) {
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember("id", tagtweet.id, allocator);
    d.AddMember("tweetID", tagtweet.tweetID, allocator);
    d.AddMember("tagID", tagtweet.tagID, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}