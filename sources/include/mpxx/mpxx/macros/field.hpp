#ifndef __MPXX_MACROS_FIELD_H__
#define __MPXX_MACROS_FIELD_H__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#define MPXX_DEFINE_COMMON_FIELD(TYPE, NAME) \
typedef \
    BOOST_PP_CAT(NAME, _field)< \
        TYPE \
    > BOOST_PP_CAT(NAME, _field_type);

#define MPXX_DEFINE_FIELD_STRUCT(TYPE, NAME) \
template <typename Type> \
struct BOOST_PP_CAT(NAME, _field) \
{ \
    typedef BOOST_PP_CAT(NAME, _field) this_type; \
    typedef Type type; \
    \
    struct BOOST_PP_CAT(NAME, _tag_type) : mpxx::tag_base {}; \
    \
    typedef BOOST_PP_CAT(NAME, _tag_type) tag_type; \
    \
    const tag_type BOOST_PP_CAT(NAME, _tag) = {}; \
    type NAME; \
    \
    constexpr BOOST_PP_CAT(NAME, _field)() \
    {} \
    \
    BOOST_PP_CAT(NAME, _field)(const this_type& other) \
    : NAME(other.NAME) \
    {} \
    \
    BOOST_PP_CAT(NAME, _field)(this_type&& other) \
    : NAME(std::move(other.NAME)) \
    {} \
    \
    constexpr BOOST_PP_CAT(NAME, _field)(type&& v) \
    : NAME{std::forward<type>(v)} \
    {} \
    \
    this_type& operator=(const this_type& other) \
    { \
        NAME = other.NAME; \
        return *this; \
    } \
    \
    this_type& operator=(this_type&& other) \
    { \
        NAME = std::move(other.NAME); \
        return *this; \
    } \
    \
    constexpr static const char* name() \
    { return BOOST_PP_STRINGIZE(NAME); } \
    \
    type& value() & \
    { return NAME; } \
    \
    const type& value() const& \
    { return NAME; } \
    \
    type&& value() && \
    { return NAME; } \
};

#define MPXX_DEFINE_FIELD(R, COMMON, FIELD) \
    MPXX_DEFINE_FIELD_STRUCT( \
        BOOST_PP_TUPLE_ELEM(2, 0, FIELD), \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD) \
    ) \
    BOOST_PP_IF( \
        COMMON, \
        MPXX_DEFINE_COMMON_FIELD, \
        BOOST_PP_TUPLE_EAT(2) \
    )( \
        BOOST_PP_TUPLE_ELEM(2, 0, FIELD), \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD) \
    )

#define MPXX_MAKE_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1, FIELD), _field)<\
        BOOST_PP_TUPLE_ELEM(2, 0, FIELD) \
    >

#define MPXX_MAKE_EXT_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_EXPAND FIELD, _field_type)

#endif // __MPXX_MACROS_FIELD_H__
