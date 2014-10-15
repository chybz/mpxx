#ifndef __MPXX_H__
#define __MPXX_H__

#include <iostream>
#include <tuple>
#include <type_traits>

#include <mpxx/msgpack.hpp>
#include <mpxx/tuple_utils.hpp>
#include <mpxx/visitors.hpp>
#include <mpxx/mstruct.hpp>
#include <mpxx/define.hpp>

namespace mpxx {

template <typename ...Fields>
struct msg : mstruct<Fields...>
{
    typedef mstruct<Fields...> base_type;
    typedef msg<Fields...> this_type;

    using base_type::base_type;

    // template <typename Packer>
    // void msgpack_pack(Packer& p) const
    // { for_each(values, pack_visitor<arg_count, Packer>(p)); }

    // void msgpack_unpack(msgpack::object o)
    // { for_each(values, unpack_visitor<arg_count>(o)); }

    // void msgpack_object(msgpack::object* o, msgpack::zone* z) const
    // { for_each(values, object_visitor<arg_count>(o, z)); }

    // void dump(std::ostream& os) const
    // { for_each(values, print_visitor<arg_count>(os)); }
};

// template <typename... Args>
// inline
// std::ostream&
// operator<<(std::ostream& os, const msg<Args...>& m)
// {
//     m.dump(os);

//     return os;
// }

} // namespace mpxx

#endif // __MPXX_H__
