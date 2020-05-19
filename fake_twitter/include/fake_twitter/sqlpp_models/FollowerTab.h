#pragma once

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/table.h>

namespace fake_twitter::sqlpp_models {
namespace TabFollower_ {
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

struct author {
    struct _alias_t {
        static constexpr const char _literal[] = "author";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T author;
            T& operator()() { return author; }
            const T& operator()() const { return author; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer>;
};

struct addresser {
    struct _alias_t {
        static constexpr const char _literal[] = "addresser";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T addresser;
            T& operator()() { return addresser; }
            const T& operator()() const { return addresser; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer>;
};

}  // namespace TabFollower_

struct TabFollower
    : sqlpp::table_t<TabFollower, TabFollower_::id, TabFollower_::author,
                     TabFollower_::addresser> {
    struct _alias_t {
        static constexpr const char _literal[] = "Follower";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T Follower;
            T& operator()() { return Follower; }
            const T& operator()() const { return Follower; }
        };
    };
};
}  // namespace fake_twitter::sqlpp_models
