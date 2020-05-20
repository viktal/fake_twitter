#pragma once

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/table.h>

namespace fake_twitter::sqlpp_models {
namespace TabTwags_ {
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
struct twit {
    struct _alias_t {
        static constexpr const char _literal[] = "twit";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T twit;
            T& operator()() { return twit; }
            const T& operator()() const { return twit; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer>;
};

struct tag {
    struct _alias_t {
        static constexpr const char _literal[] = "tag";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T tag;
            T& operator()() { return tag; }
            const T& operator()() const { return tag; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer>;
};

}  // namespace TabTwags_

struct TabTwags : sqlpp::table_t<TabTwags, TabTwags_::id, 
                                        TabTwags_::twit,
                                        TabTwags_::tag> {
    struct _alias_t {
        static constexpr const char _literal[] = "Twags";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T>
        struct _member_t {
            T Twags;
            T& operator()() { return Twags; }
            const T& operator()() const { return Twags; }
        };
    };
};
}  // namespace fake_twitter::sqlpp_models