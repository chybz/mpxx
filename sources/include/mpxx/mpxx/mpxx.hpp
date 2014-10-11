#ifndef __MPXX_H__
#define __MPXX_H__

#include <tuple>
#include <type_traits>

#include <msgpack.hpp>

#include <mpxx/tuple_utils.hpp>
#include <mpxx/tag.hpp>
#include <mpxx/visitors.hpp>

namespace mpxx {

template <typename ...Args>
struct msg
{
    typedef msg<Args...> this_type;
    typedef extract_tags<Args...> extractor;

    typedef typename extractor::tag_tuple_type tag_tuple_type;
    typedef typename extractor::value_tuple_type value_tuple_type;

    static const std::size_t arg_count = sizeof...(Args);

    msg()
    : tags(),
    values()
    {}

    msg(typename extract_tag<Args>::value_type&&... args)
    : tags(),
    values(std::forward<typename extract_tag<Args>::value_type>(args)...)
    {}

    msg(const this_type& other)
    : tags(),
    values(other.values)
    {}

    msg(this_type&& other)
    : tags(),
    values(std::move<value_tuple_type>(other.values))
    {}

    template <std::size_t I>
    typename std::tuple_element<I, value_tuple_type>::type&
    get()
    { return std::get<I>(values); }

    template <std::size_t I>
    const typename std::tuple_element<I, value_tuple_type>::type&
    get() const
    { return std::get<I>(values); }

    template <typename Tag>
    typename mpxx::tuple_element<
        Tag,
        value_tuple_type,
        tag_tuple_type
    >::type&
    get()
    { return mpxx::get<Tag>(values, tags); }

    template <typename Tag>
    const typename mpxx::tuple_element<
        Tag,
        value_tuple_type,
        tag_tuple_type
    >::type&
    get() const
    { return mpxx::get<Tag>(values, tags); }

    template <typename Packer>
    void msgpack_pack(Packer& p) const
    {
        pack_visitor<arg_count, Packer> pv(p);

        for_each(values, pv);
    }

    void msgpack_unpack(msgpack::object o)
    {
        unpack_visitor<arg_count> uv(o);

        for_each(values, uv);
    }

    void msgpack_object(msgpack::object* o, msgpack::zone* z) const
    {
        object_visitor<arg_count> ov(o, z);

        for_each(values, ov);
    }

    tag_tuple_type tags;
    value_tuple_type values;
};

} // namespace mpxx

#endif // __MPXX_H__
