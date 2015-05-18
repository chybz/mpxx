#ifndef __MPXX_VISITORS_H__
#define __MPXX_VISITORS_H__

#include <stdint.h>

#include <iostream>
#include <cstddef>
#include <type_traits>

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
/// Visit by field name, field value and position
struct name_value_pos_visit {};
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
    {
        typename std::is_enum<T>::type tag;
        (*this)(v, tag);
    }

    template <typename T>
    void operator()(T& v, const std::false_type& tag)
    { p_.pack(v); }

    template <typename T>
    void operator()(T& v, const std::true_type& tag)
    {
        using type = typename std::underlying_type<T>::type;
        p_.pack(static_cast<type>(v));
    }

    Packer& p_;
};

template <std::size_t Count>
struct unpack_visitor
{
    unpack_visitor(const msgpack::object& o)
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

        typename std::is_enum<T>::type tag;
        (*this)(v, pos, tag);
    }

    template <typename T>
    void operator()(T& v, std::size_t pos, const std::false_type& tag)
    { o_.via.array.ptr[pos].convert(&v); }

    template <typename T>
    void operator()(T& v, std::size_t pos, const std::true_type& tag)
    {
        using type = typename std::underlying_type<T>::type;
        type tv;
        o_.via.array.ptr[pos].convert(&tv);

        v = std::move(static_cast<T>(tv));
    }

    const msgpack::object& o_;
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
        o_->via.array.ptr = (msgpack::object*) z->allocate_align(
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
struct describe_visitor
{
    describe_visitor(std::ostream& os)
    : os_(os)
    {}

    void operator()(const char* name, const uint8_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":uint8"; }

    void operator()(const char* name, const int8_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":sint8"; }

    void operator()(const char* name, const uint16_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":uint16"; }

    void operator()(const char* name, const int16_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":sint16"; }

    void operator()(const char* name, const uint32_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":uint32"; }

    void operator()(const char* name, const int32_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":sint32"; }

    void operator()(const char* name, const uint64_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":uint64"; }

    void operator()(const char* name, const int64_t&, std::size_t pos)
    { os_ << sep(pos) << name << ":sint64"; }

    void operator()(const char* name, const float&, std::size_t pos)
    { os_ << sep(pos) << name << ":flt"; }

    void operator()(const char* name, const double&, std::size_t pos)
    { os_ << sep(pos) << name << ":dbl"; }

    void operator()(const char* name, const std::string&, std::size_t pos)
    { os_ << sep(pos) << name << ":string"; }

    void operator()(const char* name, const char*&, std::size_t pos)
    { os_ << sep(pos) << name << ":string"; }

    template <typename T>
    void operator()(const char* name, const T& v, std::size_t pos)
    { os_ << sep(pos) << name << ":unknown"; }

    const char* sep(std::size_t pos) const
    { return (pos > 0 ? "," : ""); }

    std::ostream& os_;
};

} // namespace mpxx

#endif // __MPXX_VISITORS_H__
