#ifndef __MPXX_MSTRUCT_H__
#define __MPXX_MSTRUCT_H__

#include <iostream>
#include <tuple>
#include <type_traits>

#include <mpxx/tuple_utils.hpp>
#include <mpxx/visitors.hpp>
#include <mpxx/define.hpp>

/// @file
///
/// Self-aware struct-like type
/// see <a href="http://duriansoftware.com/joe/Self-aware-struct-like-types-in-C++11.html">Self-aware struct-like types in C++11</a>

namespace mpxx {

/// Base tag for field tags
struct tag_base {};

/// Self-aware struct-like type (see <a href="http://duriansoftware.com/joe/Self-aware-struct-like-types-in-C++11.html">Self-aware struct-like types in C++11</a>).
///
/// You should not use this class directly but define it using MPXX_* macros.
///
/// @tparam Fields pack of field types
template <typename ...Fields>
struct mstruct : Fields...
{
    /// Commodity type for this mstruct
    typedef mstruct<Fields...> this_type;
    /// Tuple of field types
    typedef std::tuple<Fields...> fields_tuple;
    /// Tuple of field value types
    typedef std::tuple<typename Fields::type...> values_tuple;
    /// Tuple of field tag types
    typedef std::tuple<typename Fields::tag_type...> tags_tuple;
    /// Type of lookup indices to access fields by index
    typedef typename mpxx::make_tuple_indices<
        sizeof...(Fields)
    >::type indices_type;

    /// Lookup indices to access fields by index
    static constexpr indices_type indices = indices_type();
    /// Count of fields
    static constexpr std::size_t field_count = sizeof...(Fields);

    /// Use default constructor
    mstruct() = default;
    /// Use default copy constructor
    mstruct(const this_type& other)
    { *this = other; }

    /// Constructor using pack of field value types (POD types, ...)
    constexpr mstruct(typename Fields::type&&... v)
    : Fields{std::forward<typename Fields::type>(v)}...
    {}

    /// Sets values for all shared fields between this struct
    /// and other.
    /// Common fields are determined by common tag types.
    ///
    /// @tparam OtherFields type pack of other mstruct fields
    /// @param other mstruct to update from
    template <typename... OtherFields>
    this_type& operator=(const mstruct<OtherFields...>& other)
    {
        typedef typename mpxx::intersect_type_seq<
            tags_tuple,
            typename mstruct<OtherFields...>::tags_tuple
        >::type common_tags_tuple;

        update(common_tags_tuple(), other);

        return *this;
    }

    /// Obtain a tuple of references to field values looked up using the
    /// specified Tags pack
    ///
    /// @tparam Tags pack of tag types
    /// @param t pack of tags
    /// @returns tuple of value references
    ///
    /// @par Example
    /// @code
    /// auto t = m(m.field1_tag, m.field5_tag);
    ///
    /// std::get<0>(t) = a_value;
    /// std::cout << std::get<0>(t) << std::endl;
    /// @endcode
    template <
        typename... Tags,
        typename Enable = typename std::enable_if<
            mpxx::all_base_of<mpxx::tag_base, Tags...>::value
        >::type
    >
    std::tuple<
        typename mpxx::tuple_element<
            Tags,
            values_tuple,
            tags_tuple
        >::type&...
    >
    operator()(const Tags&... t)
    {
        return std::tie(
            mpxx::tuple_element<
                Tags,
                fields_tuple,
                tags_tuple
            >::type::value()...
        );
    }

    /// Obtain a tuple of const references to field values looked up using the
    /// specified Tags pack
    ///
    /// @tparam Tags pack of tag types
    /// @param t pack of tags
    /// @returns tuple of const value references
    ///
    /// @par Example
    /// @code
    /// auto t = m(m.field1_tag, m.field5_tag);
    ///
    /// std::cout << std::get<0>(t) << std::endl;
    /// @endcode
    template <
        typename... Tags,
        typename Enable = typename std::enable_if<
            mpxx::all_base_of<mpxx::tag_base, Tags...>::value
        >::type
    >
    std::tuple<
        const typename mpxx::tuple_element<
            Tags,
            values_tuple,
            tags_tuple
        >::type&...
    >
    operator()(const Tags&... t) const
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

    /// Calls the specified function for each field value
    ///
    /// @tparam F type of function
    /// @param f function to call
    ///
    /// @par Example
    /// @code
    /// struct value_printer {
    ///     template <typename T>
    ///     void operator()(T& t) {
    ///         std::cout << t << std::endl;
    ///     }
    /// };
    ///
    /// m.for_each(value_printer());
    /// @endcode
    template <typename F>
    void for_each(F&& f)
    { for_each<value_visit>(f); }

    /// Calls the specified function for each field value
    ///
    /// @tparam F type of function
    /// @param f function to call
    ///
    /// @par Example
    /// @code
    /// struct value_printer {
    ///     template <typename T>
    ///     void operator()(const T& t) {
    ///         std::cout << t << std::endl;
    ///     }
    /// };
    ///
    /// m.for_each(value_printer());
    /// @endcode
    template <typename F>
    void for_each(F&& f) const
    { for_each<value_visit>(f); }

    /// Calls the specified function for each field value.
    ///
    /// Signature of call is specified by Tag (see @ref visit_tags)
    ///
    /// @tparam Tag type of call
    /// @tparam F type of function
    /// @param f function to call
    ///
    /// @par Example
    /// @code
    /// struct value_printer {
    ///     template <typename T>
    ///     void operator()(T& t, std::size_t pos) {
    ///         std::cout << "at " << pos << ": " << t << std::endl;
    ///     }
    /// };
    ///
    /// m.for_each<value_pos_visit>(value_printer());
    /// @endcode
    template <typename Tag, typename F>
    void for_each(F&& f)
    {
        visit_each<this_type, values_tuple>(
            std::forward<this_type>(*this),
            f, Tag()
        );
    }

    /// Calls the specified function for each field value.
    ///
    /// Signature of call is specified by Tag (see @ref visit_tags)
    ///
    /// @tparam Tag type of call
    /// @tparam F type of function
    /// @param f function to call
    ///
    /// @par Example
    /// @code
    /// struct value_printer {
    ///     template <typename T>
    ///     void operator()(const T& t, std::size_t pos) {
    ///         std::cout << "at " << pos << ": " << t << std::endl;
    ///     }
    /// };
    ///
    /// m.for_each<value_pos_visit>(value_printer());
    /// @endcode
    template <typename Tag, typename F>
    void for_each(F&& f) const
    {
        visit_each<this_type const, values_tuple>(
            std::forward<this_type const>(*this),
            f, Tag()
        );
    }

private:
    friend mpxx::access;

    template <
        typename... Tags,
        typename... OtherFields,
        typename Enable = typename std::enable_if<
            mpxx::all_base_of<mpxx::tag_base, Tags...>::value
        >::type
    >
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
        ) = other(Tags()...);
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
