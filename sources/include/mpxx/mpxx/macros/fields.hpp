#ifndef __MPXX_MACROS_FIELDS_H__
#define __MPXX_MACROS_FIELDS_H__

#include <mpxx/macros/field.hpp>

/// @file
///
/// Fields definition macros

/// @brief Defines a set of fields to be shared by several mstructs or msgs
/// @hideinitializer
/// @param FIELDS a preprocessor sequence of field definitions
/// @note There's no comma between field definitions
/// @par Example
/// @code
/// MPXX_FIELDS(
///     (std::size_t, id)
///     (std::string, label, "optional default value")
/// );
/// @endcode
#define MPXX_FIELDS(...) \
    TPL_FOR_EACH_S((__VA_ARGS__), MPXX_FIELD, /* empty */)

#endif // __MPXX_MACROS_FIELDS_H__
