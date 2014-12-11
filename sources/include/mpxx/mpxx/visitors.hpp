#ifndef __MPXX_VISITORS_H__
#define __MPXX_VISITORS_H__

#include <iostream>
#include <cstddef>

#include <mpxx/msgpack.hpp>

namespace mpxx {

/// @file
///
/// standard mstruct visitors
/// @anchor visit_tags Visit tags

/// Visit by field value
struct value_visit {};
/// Visit by field position
struct pos_visit {};
/// Visit by field value and position
struct value_pos_visit {};
/// Visit by field and position
struct field_pos_visit {};

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
    void operator()(T& v)
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
    void operator()(T& v, std::size_t pos)
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
    void operator()(T& v, std::size_t pos)
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
    void operator()(const T& v, std::size_t pos)
    { os_ << (pos > 0 ? "," : "") << v; }

    std::ostream& os_;
};

} // namespace mpxx

#endif // __MPXX_VISITORS_H__
