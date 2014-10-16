#ifndef __MPXX_MSTRUCT_H__
#define __MPXX_MSTRUCT_H__

#include <iostream>
#include <tuple>
#include <type_traits>

#include <mpxx/tuple_utils.hpp>
#include <mpxx/visitors.hpp>
#include <mpxx/define.hpp>

namespace mpxx {

struct tag_base {};

template <typename ...Fields>
struct mstruct : Fields...
{
    typedef mstruct<Fields...> this_type;
    typedef std::tuple<Fields...> fields_tuple;
    typedef std::tuple<typename Fields::type...> values_tuple;
    typedef std::tuple<typename Fields::tag...> tags_tuple;
    typedef typename mpxx::make_tuple_indices<
        sizeof...(Fields)
    >::type indices_type;

    static constexpr indices_type indices = indices_type();
    static constexpr std::size_t arg_count = sizeof...(Fields);

    mstruct() = default;
    mstruct(const this_type& other) = default;

    constexpr mstruct(typename Fields::type&&... v)
    : Fields{v}...
    {}

    template <typename... OtherFields>
    void update(const mstruct<OtherFields...>& other)
    {
        typedef typename mpxx::intersect_type_seq<
            tags_tuple,
            typename mstruct<OtherFields...>::tags_tuple
        >::type common_tags_tuple;

        update(common_tags_tuple(), other);
    }

    template <typename... Tags, typename... OtherFields>
    void update(
        const std::tuple<Tags...>& tags,
        const mstruct<OtherFields...>& other
    )
    {
        std::tie(
            mpxx::tuple_element<
                Tags,
                fields_tuple,
                tags_tuple
            >::type::value()...
        ) = other(tags);
    }

    ///////////////////////////////////
    template <typename... Tags>
    std::tuple<
        typename mpxx::tuple_element<
            Tags,
            values_tuple,
            tags_tuple
        >::type&...
    >
    operator()(const std::tuple<Tags...>& t)
    {
        return std::tie(
            mpxx::tuple_element<
                Tags,
                fields_tuple,
                tags_tuple
            >::type::value()...
        );
    }

    template <typename... Tags>
    std::tuple<
        const typename mpxx::tuple_element<
            Tags,
            values_tuple,
            tags_tuple
        >::type&...
    >
    operator()(const std::tuple<Tags...>& t) const
    {
        return std::make_tuple(
            std::cref(
                mpxx::tuple_element<
                    Tags,
                    fields_tuple,
                    tags_tuple
                >::type::value()
            )...
        );
    }
    ///////////////////////////////////

    template <typename... Tags>
    std::tuple<
        typename mpxx::tuple_element<
            Tags,
            values_tuple,
            tags_tuple
        >::type&...
    >
    operator()(Tags... t)
    {
        return std::tie(
            mpxx::tuple_element<
                Tags,
                fields_tuple,
                tags_tuple
            >::type::value()...
        );
    }

    template <typename... Tags>
    std::tuple<
        const typename mpxx::tuple_element<
            Tags,
            values_tuple,
            tags_tuple
        >::type&...
    >
    operator()(Tags... t) const
    {
        return std::make_tuple(
            std::cref(
                mpxx::tuple_element<
                    Tags,
                    fields_tuple,
                    tags_tuple
                >::type::value()
            )...
        );
    }

    template <typename F>
    void for_each(F&& f)
    { for_each<value_visit>(f); }

    template <typename F>
    void for_each(F&& f) const
    { for_each<value_visit>(f); }

    template <typename Tag, typename F>
    void for_each(F&& f)
    {
        visit_each<this_type, values_tuple>(
            std::forward<this_type>(*this),
            f, Tag()
        );
    }

    template <typename Tag, typename F>
    void for_each(F&& f) const
    {
        visit_each<this_type const, values_tuple>(
            std::forward<this_type const>(*this),
            f, Tag()
        );
    }

    template <std::size_t I, typename F>
    void operator()(F&& f, value_visit tag)
    { f(std::tuple_element<I, fields_tuple>::type::value()); }

    template <std::size_t I, typename F>
    void operator()(F&& f, value_visit tag) const
    { f(std::tuple_element<I, fields_tuple>::type::value()); }

    template <std::size_t I, typename F>
    void operator()(F&& f, pos_visit tag) const
    { f(I); }

    template <std::size_t I, typename F>
    void operator()(F&& f, value_pos_visit tag)
    { f(std::tuple_element<I, fields_tuple>::type::value(), I); }

    template <std::size_t I, typename F>
    void operator()(F&& f, value_pos_visit tag) const
    { f(std::tuple_element<I, fields_tuple>::type::value(), I); }
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

#endif // __MPXX_MSTRUCT_H__
