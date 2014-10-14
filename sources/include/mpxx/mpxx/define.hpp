#ifndef __MPXX_DEFINE_H__
#define __MPXX_DEFINE_H__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq.hpp>

// Preprocessor boilerplate stolen from boost::fusion::adapt_struct
#define MPXX_FILLER_0(X, Y) ((X, Y)) MPXX_FILLER_1
#define MPXX_FILLER_1(X, Y) ((X, Y)) MPXX_FILLER_0
#define MPXX_FILLER_0_END
#define MPXX_FILLER_1_END

//
#define MPXX_DEFINE_FIELD_STRUCT(TYPE, NAME) \
struct BOOST_PP_CAT(NAME, _tag) {}; \
\
template <typename Type,typename Tag> \
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
        BOOST_PP_SEQ_ELEM(0,FIELD), \
        BOOST_PP_SEQ_ELEM(1,FIELD) \
    )

#define MPXX_FIELD(R,DATA,FIELD) BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1,FIELD),_field)<BOOST_PP_SEQ_ELEM(0,FIELD),BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1,FIELD),_tag)>

#define MPXX_DEFINE_MSG(NAME,FIELDS) \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_FIELD,~,FIELDS) \
struct NAME : mpxx::msg<> \
{ \
    \
    using base_type::base_type; \
};

#define MPXX_MSG(NAME, FIELDS) \
    MPXX_DEFINE_MSG( \
        NAME, \
        BOOST_PP_CAT(MPXX_FILLER_0 FIELDS,_END) \
    )


#ifdef MPXX_DEBUG
MPXX_MSG(
    msg_type,
    (int, id)
    (std::string, str)
    (uint64_t, count)
)
#endif // MPXX_DEBUG

#endif // __MPXX_DEFINE_H__
// typedef mpxx::msg<BOOST_PP_SEQ_TO_FIELD(BOOST_PP_SEQ_FOR_EACH(MPXX_FIELD,NAME,FIELDS))> base_type;
