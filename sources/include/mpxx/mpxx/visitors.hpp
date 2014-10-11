#ifndef __MPXX_VISITORS_H__
#define __MPXX_VISITORS_H__

#include <iostream>
#include <cstddef>

#include <msgpack.hpp>

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
        o_->type = msgpack::type::ARRAY;
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

template <std::size_t Count>
struct print_visitor
{
    print_visitor(std::ostream& os)
    : os_(os)
    {}

    template <typename T>
    void operator()(std::size_t pos, const T& v)
    { os_ << (pos > 0 ? "," : "") << v; }

    std::ostream& os_;
};

} // namespace mpxx

#endif // __MPXX_VISITORS_H__
