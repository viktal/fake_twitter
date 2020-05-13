#pragma once

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace fake_twitter::sqlpp_models
{
    namespace TabComments_
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
        struct body
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "body";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T body;
                    T& operator()()
                    {
                        return body;
                    }
                    const T& operator()() const
                    {
                        return body;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::varchar>;
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
        struct comment_for
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "comment_for";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T comment_for;
                    T& operator()()
                    {
                        return comment_for;
                    }
                    const T& operator()() const
                    {
                        return comment_for;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };
        struct create_date
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "create_date";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T create_date;
                    T& operator()()
                    {
                        return create_date;
                    }
                    const T& operator()() const
                    {
                        return create_date;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::time_point>;
        };
        struct rating
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "rating";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T rating;
                    T& operator()()
                    {
                        return rating;
                    }
                    const T& operator()() const
                    {
                        return rating;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };
    }  // namespace TabTweets_

    struct TabComments : sqlpp::table_t<TabComments, TabComments_::id,
            TabComments_::body,
            TabComments_::author,
            TabComments_::comment_for,
            TabComments_::create_date,
            TabComments_::rating>
    {
        struct _alias_t
        {
            static constexpr const char _literal[] = "Comments";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t
            {
                T Comments;
                T& operator()()
                {
                    return Comments;
                }
                const T& operator()() const
                {
                    return Comments;
                }
            };
        };
    };
}  // namespace fake_twitter::sqlpp_models

