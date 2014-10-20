#ifndef __MPXX_DEFINE_H__
#define __MPXX_DEFINE_H__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/facilities/identity.hpp>

/// @file
///
/// Message and structure definition macros

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
    > BOOST_PP_CAT(NAME, _field_type);

#define MPXX_DEFINE_FIELD_STRUCT(TYPE, NAME) \
struct BOOST_PP_CAT(NAME, _tag_type) : mpxx::tag_base {}; \
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

#define MPXX_DEFINE_FIELD_TAG(R, DATA, FIELD) \
    const BOOST_PP_CAT( \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD), \
        _tag_type \
    ) BOOST_PP_CAT( \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD), \
        _tag \
    ) = BOOST_PP_CAT( \
        BOOST_PP_TUPLE_ELEM(2, 1, FIELD), \
        _tag_type \
    )();

#define MPXX_DEFINE_EXT_FIELD_TAG(R, DATA, FIELD) \
    const BOOST_PP_CAT( \
        BOOST_PP_EXPAND FIELD, \
        _tag_type \
    ) BOOST_PP_CAT( \
        BOOST_PP_EXPAND FIELD, \
        _tag \
    ) = BOOST_PP_CAT( \
        BOOST_PP_EXPAND FIELD, \
        _tag_type \
    )();

#define MPXX_MAKE_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1, FIELD), _field)<\
        BOOST_PP_TUPLE_ELEM(2, 0, FIELD),\
        BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1, FIELD), _tag_type) \
    >

#define MPXX_MAKE_EXT_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_EXPAND FIELD, _field_type)

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
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_FIELD_TAG, ~, FIELDS) \
};

#define MPXX_DEFINE_BASE_EXT_FIELDS(BASE, NAME, FIELDS) \
struct NAME : BASE<\
    BOOST_PP_SEQ_FOR_EACH_I(MPXX_MAKE_EXT_FIELD, NAME, FIELDS) \
> \
{ \
    typedef BASE<\
        BOOST_PP_SEQ_FOR_EACH_I(MPXX_MAKE_EXT_FIELD, NAME, FIELDS) \
    > base_type; \
    using base_type::base_type; \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_EXT_FIELD_TAG, ~, FIELDS) \
};

/// @brief Defines a set of mstruct fields to be shared by several mstructs of msgs
/// @hideinitializer
/// @param FIELDS a preprocessor sequence of field definitions
/// @note There's no comma between field definitions
/// @par Example
/// @code
/// MPXX_FIELDS(
///     (std::size_t, id)
///     (std::string, label)
/// );
/// @endcode
#define MPXX_FIELDS(FIELDS) \
    MPXX_DEFINE_FIELDS( \
        BOOST_PP_CAT(MPXX_TESC_0 FIELDS,_END) \
    )

/// @brief Defines an mstruct type with the specified fields
/// @hideinitializer
/// @param NAME mstruct type name
/// @param FIELDS a preprocessor sequence of field definitions
/// @note There's no comma between field definitions
/// @par Example
/// @code
/// MPXX_STRUCT(
///     my_struct,
///     (std::size_t, id)
///     (std::string, label)
/// );
/// @endcode
#define MPXX_STRUCT(NAME,FIELDS) \
    MPXX_DEFINE_BASE( \
        mpxx::mstruct, \
        NAME, \
        BOOST_PP_CAT(MPXX_TESC_0 FIELDS,_END) \
    )

/// @brief Defines an mstruct type with the specified predefined fields
/// @hideinitializer
/// @param NAME mstruct type name
/// @param FIELDS a preprocessor sequence of already defined field types
/// @note There's no comma between field definitions
/// @par Example
/// @code
/// MPXX_FIELDS(
///     (std::size_t, id)
///     (std::string, label)
/// );
///
/// MPXX_STRUCT_EXT_FIELDS(
///     my_struct,
///     (id)
///     (label)
/// );
/// @endcode
#define MPXX_STRUCT_EXT_FIELDS(NAME, FIELDS) \
    MPXX_DEFINE_BASE_EXT_FIELDS( \
        mpxx::mstruct, \
        NAME, \
        BOOST_PP_CAT(MPXX_ESC_0 FIELDS,_END) \
    )

/// @brief Defines an msg (mstruct with MessagePack support) type with the specified fields
/// @hideinitializer
/// @param NAME message type name
/// @param FIELDS a preprocessor sequence of field definitions
/// @note There's no comma between field definitions
/// @par Example
/// @code
/// MPXX_MSG(
///     my_msg,
///     (std::size_t, id)
///     (std::string, label)
/// );
/// @endcode
#define MPXX_MSG(NAME, FIELDS) \
    MPXX_DEFINE_BASE( \
        mpxx::msg, \
        NAME, \
        BOOST_PP_CAT(MPXX_TESC_0 FIELDS,_END) \
    )

/// @brief Defines an msg type with the specified predefined fields
/// @hideinitializer
/// @param NAME message type name
/// @param FIELDS a preprocessor sequence of already defined field types
/// @note There's no comma between field definitions
/// @par Example
/// @code
/// MPXX_FIELDS(
///     (std::size_t, id)
///     (std::string, label)
/// );
///
/// MPXX_MSG_EXT_FIELDS(
///     my_struct,
///     (id)
///     (label)
/// );
/// @endcode
#define MPXX_MSG_EXT_FIELDS(NAME, FIELDS) \
    MPXX_DEFINE_BASE_EXT_FIELDS( \
        mpxx::msg, \
        NAME, \
        BOOST_PP_CAT(MPXX_ESC_0 FIELDS,_END) \
    )

#endif // __MPXX_DEFINE_H__
