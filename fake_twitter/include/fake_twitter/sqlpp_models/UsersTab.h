#pragma once

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace fake_twitter::sqlpp_models
{
    namespace TabUsers_
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
        struct name
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "name";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T name;
                    T& operator()()
                    {
                        return name;
                    }
                    const T& operator()() const
                    {
                        return name;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::varchar>;
        };
        struct username
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "username";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T username;
                    T& operator()()
                    {
                        return username;
                    }
                    const T& operator()() const
                    {
                        return username;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::varchar>;
        };
        struct password_hash
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "password_hash";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T password_hash;
                    T& operator()()
                    {
                        return password_hash;
                    }
                    const T& operator()() const
                    {
                        return password_hash;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };
        struct avatar
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "avatar";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T avatar;
                    T& operator()()
                    {
                        return avatar;
                    }
                    const T& operator()() const
                    {
                        return avatar;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::varchar>;
        };
        struct followers_count
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "followers_count";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T followers_count;
                    T& operator()()
                    {
                        return followers_count;
                    }
                    const T& operator()() const
                    {
                        return followers_count;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };
        struct friends_count
        {
            struct _alias_t
            {
                static constexpr const char _literal[] = "friends_count";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t
                {
                    T friends_count;
                    T& operator()()
                    {
                        return friends_count;
                    }
                    const T& operator()() const
                    {
                        return friends_count;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };
    }  // namespace TabFoo_

    struct TabUsers : sqlpp::table_t<TabUsers, TabUsers_::id,
                                               TabUsers_::name,
                                               TabUsers_::username,
                                               TabUsers_::avatar,
                                               TabUsers_::password_hash,
                                               TabUsers_::followers_count,
                                               TabUsers_::friends_count>
    {
        struct _alias_t
        {
            static constexpr const char _literal[] = "Users";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t
            {
                T Users;
                T& operator()()
                {
                    return Users;
                }
                const T& operator()() const
                {
                    return Users;
                }
            };
        };
    };
}  // namespace test
