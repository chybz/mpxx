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
#define MPXX_MSG(NAME, ...) \
    MPXX_DEFINE_BASE( \
        mpxx::msg, \
        NAME, \
        __VA_ARGS__ \
    )

#ifdef MPXX_DEBUG
MPXX_FIELDS(
    (std::size_t, counter, 1234),
    (bool, valid, false),
    (std::string, label, "some string"),
    (double, avg, 1.0)
);

MPXX_STRUCT(
    my_struct,
    (counter),
    (std::size_t, size, 42),
    (std::string, message),
    (valid),
    (float, balance, 100.0)
);
#endif // MPXX_DEBUG

#endif // __MPXX_DEFINE_H__
