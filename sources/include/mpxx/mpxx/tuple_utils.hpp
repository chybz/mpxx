#ifndef __MPXX_TUPLE_UTILS_H__
#define __MPXX_TUPLE_UTILS_H__

#include <stdint.h>

#include <type_traits>
#include <tuple>

#define RETURNS(x) -> decltype(x) { return x; }

namespace mpxx {

template <int ...>
struct tuple_indices
{};

template <int N, int ...I>
struct make_tuple_indices
: make_tuple_indices<N - 1, N - 1, I...>
{};

template<int ...I>
struct make_tuple_indices<0, I...> {
    typedef tuple_indices<I...> type;
};

namespace detail {

template <std::size_t I, class Tuple, typename F, typename ...Args>
struct for_each_impl {
    static void for_each(Tuple&& t, F& f, Args&&... args)
    {
        for_each_impl<I - 1, Tuple, F, Args...>::for_each(
            t, f, std::forward<Args>(args)...
        );
        f(I, std::get<I>(t), std::forward<Args>(args)...);
    }
};

template <class Tuple, typename F, typename... Args>
struct for_each_impl<0, Tuple, F, Args...> {
    static void for_each(Tuple&& t, F& f, Args&&... args)
    {
        f(0, std::get<0>(t), std::forward<Args>(args)...);
    }
};

template <int Index, typename Search, typename First, typename... Types>
struct get_impl
{
    typedef typename get_impl<Index + 1, Search, Types...>::type type;
    static constexpr int index = Index;
};

template <int Index, typename Search, typename... Types>
struct get_impl<Index, Search, Search, Types...>
{
    typedef get_impl type;
    static constexpr int index = Index;
};

} // namespace detail

template<class Tuple, typename F, typename... Args>
void for_each(Tuple&& t, F& f, Args&&... args)
{
    detail::for_each_impl<
        std::tuple_size<
            typename std::decay<Tuple>::type
        >::value - 1,
        Tuple, F, Args...
    >::for_each(t, f, std::forward<Args>(args)...);
}

template <typename T, typename... Types>
struct tuple_element;

template <typename T, typename... Types, typename... Keys>
struct tuple_element<T, std::tuple<Types...>, std::tuple<Keys...>>
{
    typedef typename std::tuple_element<
        detail::get_impl<0, T, Keys...>::type::index,
        std::tuple<Types...>
    >::type type;
};

template <typename T, typename... Types>
T& get(std::tuple<Types...>& tuple)
{
    return std::get<
        detail::get_impl<0, T, Types...>::type::index
    >(tuple);
}

template <typename T, typename... Types>
const T& get(const std::tuple<Types...>& tuple)
{
    return std::get<
        detail::get_impl<0, T, Types...>::type::index
    >(tuple);
}

template <typename T, typename... Types, typename... Keys>
typename tuple_element<
    T,
    std::tuple<Types...>,
    std::tuple<Keys...>
>::type&
get(std::tuple<Types...>& tuple, std::tuple<Keys...>& keys)
{
    return std::get<
        detail::get_impl<0, T, Keys...>::type::index
    >(tuple);
}

template <typename T, typename... Types, typename... Keys>
const typename tuple_element<
    T,
    std::tuple<Types...>,
    std::tuple<Keys...>
>::type&
get(const std::tuple<Types...>& tuple, const std::tuple<Keys...>& keys)
{
    return std::get<
        detail::get_impl<0, T, Keys...>::type::index
    >(tuple);
}

} // namespace mpxx

#endif // __MPXX_TUPLE_UTILS_H__
