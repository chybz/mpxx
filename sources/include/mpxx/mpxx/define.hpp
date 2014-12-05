#ifndef __MPXX_DEFINE_H__
#define __MPXX_DEFINE_H__

#include <mpxx/tpl/tpl.hpp>
#include <mpxx/macros/fields.hpp>
#include <mpxx/macros/define_base.hpp>

/// @file
///
/// Message and structure definition macros

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
#define MPXX_STRUCT(NAME, ...) \
    MPXX_DEFINE_BASE( \
        mpxx::mstruct, \
        NAME, \
        __VA_ARGS__ \
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
        BOOST_PP_CAT(MPXX_TESC2_0 FIELDS,_END) \
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
// MPXX_FIELDS(
//     (std::size_t, counter, 1234),
//     (bool, valid, false),
//     (std::string, label, "some string"),
//     (double, avg, 1.0)
// );

MPXX_STRUCT(
    my_struct,
    (std::size_t, counter, 1234),
    (bool, valid, false),
    (std::string, label, "some string"),
    (double, avg, 1.0)
);

// MPXX_STRUCT_EXT_FIELDS(
//     mstruct1,
//     (valid)
//     (avg)
// );
#endif // MPXX_DEBUG

#endif // __MPXX_DEFINE_H__
