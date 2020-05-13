#pragma once

#include "fake_twitter/model/User.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <fake_twitter/model/Tweet.h>

using namespace rapidjson;

namespace fake_twitter::serialization {

    std::string to_json(model::User user) {
        Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

        d.AddMember("id", user.id, allocator);
        d.AddMember("name", Value().SetString(StringRef(user.name.c_str())), allocator);
        d.AddMember("username", Value().SetString(StringRef(user.username.c_str())), allocator);
        d.AddMember("followers_count", user.followers_count, allocator);
        d.AddMember("friends_count", user.friends_count, allocator);
        d.AddMember("avatar", Value().SetString(StringRef(user.avatar.c_str())), allocator);
        d.AddMember("password_hash", user.password_hash, allocator);

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);

        return buffer.GetString();
    }


    template<typename T>
    T from_json(const std::string& json);

    // TODO: Сделать user классом и у него статический метод from_json или как-то еще.
    template<>
    model::User from_json<model::User>(const std::string& json)
    {
        Document document;
        document.Parse(json.c_str());
        return model::User{
            document["id"].Get<PKey>(),
            document["name"].GetString(),
            document["username"].GetString(),
            document["password_hash"].Get<PasswordHash>(),
            document["avatar"].GetString(),
            document["followers_count"].Get<size_t>(),
            document["friends_count"].Get<size_t>()
        };
    }

    std::string to_json(model::Tweet tweet) {
        Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

        d.AddMember("id", tweet.id, allocator);
        d.AddMember("name", Value().SetString(StringRef(tweet.body.c_str())), allocator);
        d.AddMember("author", tweet.author, allocator);
        d.AddMember("create_date", Value().SetString(StringRef(tweet.create_date.c_str())), allocator);
        d.AddMember("retweets", tweet.retweets, allocator);
        d.AddMember("rating", tweet.rating, allocator);

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);

//        return "";
        return buffer.GetString();
    }

} //fake_twitter

