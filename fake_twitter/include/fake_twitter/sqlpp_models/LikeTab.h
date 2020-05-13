#pragma once

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace fake_twitter::sqlpp_models
{
    namespace TabLikes_
    {
        struct id
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "id";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T id;
                    T& operator()()
                    {
                        return id;
                    }
                    const T& operator()() const
                    {
                        return id;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
        };

        struct author
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "author";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T author;
                    T& operator()()
                    {
                        return author;
                    }
                    const T& operator()() const
                    {
                        return author;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };

        struct twit
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "twit";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T twit;
                    T& operator()()
                    {
                        return twit;
                    }
                    const T& operator()() const
                    {
                        return twit;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };
        
    }  // namespace TabLikes_

    struct TabLikes : sqlpp::table_t<TabLikes, TabLikes_::id,
            TabLikes_::author,
            TabLikes_::twit>
    {
        struct _alias_t
        {
            static constexpr const char _literal[] = "Likes";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t
            {
                T Likes;
                T& operator()()
                {
                    return Likes;
                }
                const T& operator()() const
                {
                    return Likes;
                }
            };
        };
    };
}  // namespace fake_twitter::sqlpp_models
