#ifndef __MPXX_H__
#define __MPXX_H__

#include <tuple>

#include <msgpack.hpp>

#include <mpxx/tuple_utils.hpp>

namespace mpxx {

template <
    std::size_t Count,
    typename Packer
>
struct pack_visitor
{
    pack_visitor(Packer& p)
    : p_(p)
    { p_.pack_array(Count); }

    template <typename T>
    void operator()(std::size_t pos, T& v)
    { p_.pack(v); }

    Packer& p_;
};

template <std::size_t Count>
struct unpack_visitor
{
    unpack_visitor(msgpack::object& o)
    : o_(o)
    {
        if (o_.type != msgpack::type::ARRAY) {
            throw msgpack::type_error();
        }

        size_ = o_.via.array.size;
    }

    template <typename T>
    void operator()(std::size_t pos, T& v)
    {
        if (pos > size_) {
            return;
        }

        o_.via.array.ptr[pos].convert(&v);
    }

    msgpack::object& o_;
    std::size_t size_;
};

template <std::size_t Count>
struct object_visitor
{
    object_visitor(msgpack::object* o, msgpack::zone* z)
    : o_(o),
    z_(z)
    {
        o_->type = type::ARRAY;
        o_->via.array.ptr = (msgpack::object*) z->malloc(
            sizeof(msgpack::object) * Count
        );
        o_->via.array.size = Count;
    }

    template <typename T>
    void operator()(std::size_t pos, T& v)
    { o_->via.array.ptr[pos] = object(v, z_); }

    msgpack::object* o_;
    msgpack::zone* z_;
};

template <typename ...Args>
struct define
{
    typedef define<Args...> value_type;
    typedef std::tuple<Args...> tuple_type;

    static constexpr std::size_t arg_count =
        std::tuple_size<tuple_type>::value;

    define(Args&&... args)
    : a(std::forward<Args>(args)...)
    {}

    template <typename Packer>
    void msgpack_pack(Packer& p) const
    {
        pack_visitor<arg_count, Packer> pv(p);

        for_each(a, pv);
    }

    void msgpack_unpack(msgpack::object o)
    {
        unpack_visitor<arg_count> uv(o);

        for_each(a, uv);
    }

    void msgpack_object(msgpack::object* o, msgpack::zone* z) const
    {
        object_visitor<arg_count> ov(o, z);

        for_each(a, ov);
    }

    tuple_type a;
};

} // namespace mpxx

#endif // __MPXX_H__
