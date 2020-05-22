#pragma once

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/table.h>

namespace fake_twitter::sqlpp_models {
namespace TabTags_ {
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
struct title {
    struct _alias_t {
        static constexpr const char _literal[] = "title";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T title;
            T& operator()() { return title; }
            const T& operator()() const { return title; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::varchar>;
};
}  // namespace TabTags_

struct TabTags : sqlpp::table_t<TabTags, TabTags_::id, TabTags_::title> {
    struct _alias_t {
        static constexpr const char _literal[] = "Tags";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T Tags;
            T& operator()() { return Tags; }
            const T& operator()() const { return Tags; }
        };
    };
};
}  // namespace fake_twitter::sqlpp_models