#include <stdlib.h>

#include <cxxabi.h>

#include <mpxx/utils.hpp>

namespace mpxx {

std::string
demangle_type_name(const std::string& mangled)
{
    char* buffer;
    int status;

    buffer = abi::__cxa_demangle(mangled.c_str(), 0, 0, &status);

    if (status == 0) {
        std::string n(buffer);
        free(buffer);

        return n;
    } else {
        return std::string("demangle failure");
    }

    return std::string("unsupported");
}

} // namespace mpxx
