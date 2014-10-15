#ifndef __MPXX_MSTRUCT_H__
#define __MPXX_MSTRUCT_H__

#include <iostream>
#include <tuple>
#include <type_traits>

#include <mpxx/tuple_utils.hpp>
#include <mpxx/visitors.hpp>
#include <mpxx/define.hpp>

namespace mpxx {

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
    {
        visit_each<this_type, values_tuple>(*this, f);
    }

    template <std::size_t I, typename F>
    void operator()(F&& f)
    {
        f(std::tuple_element<I, fields_tuple>::type::value());
    }
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
