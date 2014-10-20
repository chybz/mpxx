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

    template <typename Packer>
    void msgpack_pack(Packer& p) const
    {
        base_type::for_each(
            pack_visitor<base_type::field_count, Packer>(p)
        );
    }

    void msgpack_unpack(msgpack::object o)
    {
        base_type::template for_each<value_pos_visit>(
            unpack_visitor<base_type::field_count>(o)
        );
    }

    void msgpack_object(msgpack::object* o, msgpack::zone* z) const
    {
        base_type::template for_each<value_pos_visit>(
            object_visitor<base_type::field_count>(o, z)
        );
    }

    void dump(std::ostream& os) const
    {
        base_type::template for_each<value_pos_visit>(
            print_visitor<base_type::field_count>(os)
        );
    }
};

template <typename... Args>
inline
std::ostream&
operator<<(std::ostream& os, const msg<Args...>& m)
{
    m.dump(os);

    return os;
}

} // namespace mpxx

#endif // __MPXX_H__
