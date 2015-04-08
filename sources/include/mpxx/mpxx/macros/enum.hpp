#ifndef __MPXX_MACROS_ENUM_H__
#define __MPXX_MACROS_ENUM_H__

#include <type_traits>

#include <mpxx/msgpack.hpp>

/// @file
///
/// Enum pack/unpack adapters

/// @brief Defines MessagePack custom converters for an enum
/// @hideinitializer
/// @param ENUM enum type
/// @par Example
/// @code
/// namespace some_ns {
///
/// enum class my_enum
/// {
///     first,
///     second,
///     ...
/// };
///
/// } // namespace some_ns
///
/// MPXX_ADAPT_ENUM(some_ns::my_enum)
/// @endcode
#define MPXX_ADAPT_ENUM(ENUM) MSGPACK_ADD_ENUM(ENUM)

#endif // __MPXX_MACROS_ENUM_H__
