#ifndef __MPXX_MACROS_DEFINE_BASE_H__
#define __MPXX_MACROS_DEFINE_BASE_H__

#include <mpxx/macros/fields.hpp>

#define MPXX_DEFINE_BASE(BASE, NAME, ...) \
    TPL_FOR_EACH_S((__VA_ARGS__), MPXX_FIELD, NAME) \
typedef BASE< \
    TPL_FOR_EACH_L((__VA_ARGS__), MPXX_FIELD_TYPE, NAME) \
> NAME;

#endif // __MPXX_MACROS_DEFINE_BASE_H__
