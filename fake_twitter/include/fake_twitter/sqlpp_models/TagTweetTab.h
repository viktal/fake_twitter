#pragma once

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/table.h>

namespace fake_twitter::sqlpp_models {
namespace TabTagTweet_ {
struct id {
    struct _alias_t {
        static constexpr const char _literal[] = "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert,
                           sqlpp::tag::must_not_update>;
};

struct tweetID {
    struct _alias_t {
        static constexpr const char _literal[] = "tweetID";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T tweetID;
            T& operator()() { return tweetID; }
            const T& operator()() const { return tweetID; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer>;
};

struct tagID {
    struct _alias_t {
        static constexpr const char _literal[] = "tagID";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T tagID;
            T& operator()() { return tagID; }
            const T& operator()() const { return tagID; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer>;
};

}  // namespace TabTagTweet_

struct TabTagTweet
    : sqlpp::table_t<TabTagTweet, TabTagTweet_::id, TabTagTweet_::tweetID,
                     TabTagTweet_::tagID> {
    struct _alias_t {
        static constexpr const char _literal[] = "TagTweet";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T TagTweet;
            T& operator()() { return TagTweet; }
            const T& operator()() const { return TagTweet; }
        };
    };
};
}  // namespace fake_twitter::sqlpp_models
