#pragma once

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <stdio.h>

#include <vector>

#include "fake_twitter/model/TagTweet.h"
#include "fake_twitter/model/Tag.h"
#include "fake_twitter/model/Comment.h"
#include "fake_twitter/model/Followers.h"
#include "fake_twitter/model/Tweet.h"
#include "fake_twitter/model/User.h"

namespace fake_twitter::serialization {

std::string to_json(model::User user);

template <typename T>
T from_json(const std::string& json);

std::string to_json(std::vector<model::Tweet> tweetVector);
std::string to_json(model::Followers followers);
std::string to_json(model::Tweet tweet);
std::string to_json(model::Followers followers);
std::string to_json(model::Comment comment);
std::string to_json(model::Comment* comment, int number);
std::string to_json(model::Tag tag);
std::string to_json(model::TagTweet tagtweet);
}  // namespace fake_twitter::serialization
