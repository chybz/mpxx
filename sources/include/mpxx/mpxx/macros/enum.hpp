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
#define MPXX_ADAPT_ENUM(ENUM)                                                 \
namespace msgpack {                                                           \
                                                                              \
inline                                                                        \
ENUM&                                                                         \
operator>>(object const& o, ENUM& v)                                          \
{                                                                             \
    using utype = typename std::underlying_type<ENUM>::type;                  \
    utype uv;                                                                 \
                                                                              \
    o.convert(&uv);                                                           \
    v = static_cast<ENUM>(uv);                                                \
}                                                                             \
                                                                              \
template <typename Stream>                                                    \
inline                                                                        \
packer<Stream>&                                                               \
operator<<(packer<Stream>& o, const ENUM& v)                                  \
{                                                                             \
    using utype = typename std::underlying_type<T>::type;                     \
                                                                              \
    o.pack(static_cast<utype>(v));                                            \
                                                                              \
    return o;                                                                 \
}

} // namespace msgpack

#endif // __MPXX_MACROS_ENUM_H__
