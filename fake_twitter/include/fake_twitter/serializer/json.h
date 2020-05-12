#pragma once

#include <stdio.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace fake_twitter::serialization {


    std::string to_json(model::Comment comment) {
        using namespace rapidjson;

        Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

        d.AddMember("id", comment.id, allocator);
        d.AddMember("name", Value().SetString(StringRef(comment.body.c_str())), allocator);
        d.AddMember("author", comment.author, allocator);
        d.AddMember("create_date", Value().SetString(StringRef(comment.create_date.c_str())), allocator);
        d.AddMember("comment_for", comment.comment_for, allocator);
        d.AddMember("rating", comment.rating, allocator);

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);

//        return "";
        return buffer.GetString();
    }

    std::string to_json(model::Comment* comment, int number) {
        using namespace rapidjson;

        Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
        char table[6][32] = {"id", "name", "author", "create_date","comment_for", "rating" };
        for(int i = 0; i <= number; i++) {
            for (int j = 0; j < 6; j++) {
                char c[16];
                sprintf(c, "%d", i+1);
                strcat(table[j],c);
            }
            d.AddMember(Value().SetString(StringRef(table[0])), comment[i].id, allocator);
            d.AddMember(Value().SetString(StringRef(table[1])), Value().SetString(StringRef(comment[i].body.c_str())), allocator);
            d.AddMember(Value().SetString(StringRef(table[2])), comment[i].author, allocator);
            d.AddMember(Value().SetString(StringRef(table[3])), Value().SetString(StringRef(comment[i].create_date.c_str())), allocator);
            d.AddMember(Value().SetString(StringRef(table[4])), comment[i].comment_for, allocator);
            d.AddMember(Value().SetString(StringRef(table[5])), comment[i].rating, allocator);

        }

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);

//        return "";
        return buffer.GetString();
    }
} //fake_twitter

