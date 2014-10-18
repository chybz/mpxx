#ifndef __MPXX_ACCESS_H__
#define __MPXX_ACCESS_H__

#include <cstddef>
#include <utility>

namespace mpxx {

class access
{
public:
    template <
        std::size_t I,
        typename Class,
        typename... Args
    >
    static
    void call(Class& c, Args&&... args)
    { c.template operator()<I>(std::forward<Args>(args)...); }

private:
    access();
};

} // namespace mpxx

#endif // __MPXX_ACCESS_H__
