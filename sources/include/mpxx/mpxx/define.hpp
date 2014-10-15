#ifndef __MPXX_DEFINE_H__
#define __MPXX_DEFINE_H__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

// Preprocessor boilerplate stolen from boost::fusion::adapt_struct
#define MPXX_FILLER_0(X, Y) \
    ((X, Y)) MPXX_FILLER_1
#define MPXX_FILLER_1(X, Y) \
    ((X, Y)) MPXX_FILLER_0
#define MPXX_FILLER_0_END
#define MPXX_FILLER_1_END

//
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

#define MPXX_DEFINE_FIELD(R, DATA, FIELD) \
    MPXX_DEFINE_FIELD_STRUCT( \
        BOOST_PP_TUPLE_ELEM(2, 0, FIELD), \
        BOOST_PP_TUPLE_ELEM(2, 1,FIELD) \
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

#define MPXX_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1,FIELD),_field)<\
        BOOST_PP_TUPLE_ELEM(2, 0,FIELD),\
        BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1,FIELD),_tag_type) \
    >

#define MPXX_DEFINE_BASE(BASE, NAME, FIELDS) \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_FIELD, ~, FIELDS) \
struct NAME : BASE<\
    BOOST_PP_SEQ_FOR_EACH_I(MPXX_FIELD, NAME, FIELDS) \
> \
{ \
    typedef BASE<\
        BOOST_PP_SEQ_FOR_EACH_I(MPXX_FIELD, NAME, FIELDS) \
    > base_type; \
    using base_type::base_type; \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_TAG, ~, FIELDS) \
};

#define MPXX_STRUCT(NAME,FIELDS) \
    MPXX_DEFINE_BASE( \
        mpxx::mstruct, \
        NAME, \
        BOOST_PP_CAT(MPXX_FILLER_0 FIELDS,_END) \
    )

#define MPXX_MSG(NAME,FIELDS) \
    MPXX_DEFINE_BASE( \
        mpxx::msg, \
        NAME, \
        BOOST_PP_CAT(MPXX_FILLER_0 FIELDS,_END) \
    )

#endif // __MPXX_DEFINE_H__
