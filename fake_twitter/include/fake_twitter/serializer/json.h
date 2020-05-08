#pragma once

//#include "fake_twitter/model/User.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace fake_twitter {
namespace serialization {

    std::string to_json(model::User user) {
        using namespace rapidjson;

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

} //serialization
} //fake_twitter

