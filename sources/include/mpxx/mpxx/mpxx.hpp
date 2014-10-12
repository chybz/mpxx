#ifndef __MPXX_H__
#define __MPXX_H__

#include <iostream>
#include <tuple>
#include <type_traits>

#include <mpxx/msgpack.hpp>
#include <mpxx/tuple_utils.hpp>
#include <mpxx/tag.hpp>
#include <mpxx/visitors.hpp>
#include <mpxx/define.hpp>

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

    template <
        std::size_t... Indices,
        typename std::enable_if<(sizeof...(Indices) > 1)>::type
    >
    std::tuple<
        typename std::tuple_element<
            Indices,
            value_tuple_type
        >::type&...
    >
    get()
    { return std::tie(std::get<Indices>(values)...); }

    template <
        std::size_t... Indices,
        typename std::enable_if<(sizeof...(Indices) > 1)>::type
    >
    std::tuple<
        const typename std::tuple_element<
            Indices,
            value_tuple_type
        >::type&...
    >
    get() const
    { return std::make_tuple(std::cref(std::get<Indices>(values))...); }

    template <std::size_t... Indices>
    void set(
        const typename std::tuple_element<
            Indices,
            value_tuple_type
        >::type&... vals
    )
    { std::tie(std::get<Indices>(values)...) = std::tie(vals...); }

    template <std::size_t... Indices>
    void set(
        typename std::tuple_element<
            Indices,
            value_tuple_type
        >::type&&... vals
    )
    {
        std::tie(std::get<Indices>(values)...) =
            std::forward_as_tuple(vals...);
    }

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

    template <
        typename... Tags,
        typename Enable = typename std::enable_if<(sizeof...(Tags) > 1)>::type
    >
    std::tuple<
        typename mpxx::tuple_element<
            Tags,
            value_tuple_type,
            tag_tuple_type
        >::type&...
    >
    get()
    { return std::tie(mpxx::get<Tags>(values, tags)...); }

    template <
        typename... Tags,
        typename std::enable_if<(sizeof...(Tags) > 1)>::type
    >
    std::tuple<
        const typename mpxx::tuple_element<
            Tags,
            value_tuple_type,
            tag_tuple_type
        >::type&...
    >
    get()
    { return std::make_tuple(std::cref(mpxx::get<Tags>(values, tags))...); }

    template <typename... Tags>
    void set(
        const typename mpxx::tuple_element<
            Tags,
            value_tuple_type,
            tag_tuple_type
        >::type&... vals
    )
    { std::tie(mpxx::get<Tags>(values, tags)...) = std::tie(vals...); }

    template <typename... Tags>
    void set(
        typename mpxx::tuple_element<
            Tags,
            value_tuple_type,
            tag_tuple_type
        >::type&&... vals
    )
    {
        std::tie(mpxx::get<Tags>(values, tags)...) =
            std::forward_as_tuple(vals...);
    }

    template <typename Tag>
    typename mpxx::tuple_element<
        Tag,
        value_tuple_type,
        tag_tuple_type
    >::type&
    operator[](Tag t)
    { return mpxx::get<Tag>(values, tags); }

    template <typename... Tags>
    std::tuple<
        typename mpxx::tuple_element<
            Tags,
            value_tuple_type,
            tag_tuple_type
        >::type&...
    >
    operator()(Tags... t)
    { return get<Tags...>(); }

    template <typename... Tags>
    std::tuple<
        const typename mpxx::tuple_element<
            Tags,
            value_tuple_type,
            tag_tuple_type
        >::type&...
    >
    operator()(Tags... t) const
    { return get<Tags...>(); }

    template <typename Tag>
    const typename mpxx::tuple_element<
        Tag,
        value_tuple_type,
        tag_tuple_type
    >::type&
    operator[](Tag t) const
    { return mpxx::get<Tag>(values, tags); }

    template <typename Packer>
    void msgpack_pack(Packer& p) const
    { for_each(values, pack_visitor<arg_count, Packer>(p)); }

    void msgpack_unpack(msgpack::object o)
    { for_each(values, unpack_visitor<arg_count>(o)); }

    void msgpack_object(msgpack::object* o, msgpack::zone* z) const
    { for_each(values, object_visitor<arg_count>(o, z)); }

    void dump(std::ostream& os) const
    { for_each(values, print_visitor<arg_count>(os)); }

    tag_tuple_type tags;
    value_tuple_type values;
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
