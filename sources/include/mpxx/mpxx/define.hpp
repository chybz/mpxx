#ifndef __MPXX_DEFINE_H__
#define __MPXX_DEFINE_H__

#include <boost/preprocessor/cat.hpp>

#include <mpxx/macros/define_base.hpp>
#include <mpxx/macros/escape.hpp>

/// @file
///
/// Message and structure definition macros

/// @brief Defines a set of fields to be shared by several mstructs or msgs
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

#ifdef MPXX_DEBUG
MPXX_FIELDS(
    (std::size_t, counter)
    (bool, valid)
    (std::string, label)
    (double, avg)
);

MPXX_STRUCT_EXT_FIELDS(
    mstruct1,
    (valid)
    (avg)
);
#endif // MPXX_DEBUG

#endif // __MPXX_DEFINE_H__
