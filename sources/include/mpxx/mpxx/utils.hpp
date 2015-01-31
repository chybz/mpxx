#ifndef __MPXX_UTILS_H__
#define __MPXX_UTILS_H__

#include <string>

#include <mpxx/config.h>

namespace mpxx {

MPXX_API
std::string
demangle_type_name(const std::string& mangled);

} // namespace mpxx

#endif // __MPXX_UTILS_H__
