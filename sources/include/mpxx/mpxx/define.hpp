#ifndef __MPXX_DEFINE_H__
#define __MPXX_DEFINE_H__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

// Preprocessor boilerplate stolen from boost::fusion::adapt_struct
#define MPXX_TESC_0(X, Y) \
    ((X, Y)) MPXX_TESC_1
#define MPXX_TESC_1(X, Y) \
    ((X, Y)) MPXX_TESC_0
#define MPXX_TESC_0_END
#define MPXX_TESC_1_END

#define MPXX_ESC_0(X) \
    ((X)) MPXX_ESC_1
#define MPXX_ESC_1(X) \
    ((X)) MPXX_ESC_0
#define MPXX_ESC_0_END
#define MPXX_ESC_1_END

#define MPXX_DEFINE_COMMON_FIELD(TYPE, NAME) \
typedef \
    BOOST_PP_CAT(NAME, _field)< \
        TYPE, \
        BOOST_PP_CAT(NAME, _tag_type) \
    > NAME;

#define MPXX_DEFINE_FIELD_STRUCT(TYPE, NAME) \
struct BOOST_PP_CAT(NAME, _tag_type) {}; \
\
template <typename Type, typename Tag> \
struct BOOST_PP_CAT(NAME, _field) \
{ \
    typedef Type type; \
    typedef Tag tag; \
    \
    type NAME; \
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

#define MPXX_DEFINE_TAG(R, DATA, FIELD) \
    static constexpr BOOST_PP_CAT( \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD), \
        _tag_type \
    ) BOOST_PP_CAT( \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD), \
        _tag \
    ) = BOOST_PP_CAT( \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD), \
        _tag_type \
    )();

#define MPXX_MAKE_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1,FIELD),_field)<\
        BOOST_PP_TUPLE_ELEM(2, 0,FIELD),\
        BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1,FIELD),_tag_type) \
    >

#define MPXX_DEFINE_FIELDS(FIELDS) \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_FIELD, 1, FIELDS)

#define MPXX_DEFINE_BASE(BASE, NAME, FIELDS) \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_FIELD, 0, FIELDS) \
struct NAME : BASE<\
    BOOST_PP_SEQ_FOR_EACH_I(MPXX_MAKE_FIELD, NAME, FIELDS) \
> \
{ \
    typedef BASE<\
        BOOST_PP_SEQ_FOR_EACH_I(MPXX_MAKE_FIELD, NAME, FIELDS) \
    > base_type; \
    using base_type::base_type; \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_TAG, ~, FIELDS) \
};

#define MPXX_DEFINE_BASE_FIELDS(BASE, NAME, FIELDS) \
struct NAME : BASE<\
    BOOST_PP_SEQ_ENUM(FIELDS) \
> \
{ \
    typedef BASE<\
        BOOST_PP_SEQ_ENUM(FIELDS) \
    > base_type; \
    using base_type::base_type; \
};

#define MPXX_FIELDS(FIELDS) \
    MPXX_DEFINE_FIELDS( \
        BOOST_PP_CAT(MPXX_TESC_0 FIELDS,_END) \
    )

#define MPXX_STRUCT(NAME,FIELDS) \
    MPXX_DEFINE_BASE( \
        mpxx::mstruct, \
        NAME, \
        BOOST_PP_CAT(MPXX_TESC_0 FIELDS,_END) \
    )

#define MPXX_STRUCT_EXT_FIELDS(NAME, FIELDS) \
    MPXX_DEFINE_BASE_FIELDS( \
        mpxx::mstruct, \
        NAME, \
        BOOST_PP_CAT(MPXX_ESC_0 FIELDS,_END) \
    )

#define MPXX_MSG(NAME, FIELDS) \
    MPXX_DEFINE_BASE( \
        mpxx::msg, \
        NAME, \
        BOOST_PP_CAT(MPXX_TESC_0 FIELDS,_END) \
    )

#ifdef MPXX_DEBUG
MPXX_FIELDS(
    (std::size_t, toto)
    (std::string, titi)
);

MPXX_STRUCT_EXT_FIELDS(
    msg_type,
    (toto)
    (titi)
)
#endif

#endif // __MPXX_DEFINE_H__
