#ifndef __MPXX_TAG_H__
#define __MPXX_TAG_H__

#include <type_traits>

namespace mpxx {

namespace detail {

template <typename Tag, bool>
struct extract_tag_impl {};

template <typename Tag>
struct extract_tag_impl<Tag, true>
{
    typedef typename Tag::type type;
    typedef typename Tag::value_type value_type;
};

template <typename Tag>
struct extract_tag_impl<Tag, false>
{
    typedef struct {} type;
    typedef Tag value_type;
};

} // namespace detail

struct tag_base {};

template <typename Tag, typename T>
struct tag : tag_base
{
    typedef Tag type;
    typedef T value_type;
};

template <typename Tag>
struct extract_tag
{
    typedef detail::extract_tag_impl<
        Tag,
        std::is_base_of<tag_base, Tag>::value
    > extractor;

    typedef typename extractor::type type;
    typedef typename extractor::value_type value_type;
};

template <typename... Tags>
struct extract_tags
{
    typedef std::tuple<
        typename extract_tag<Tags>::type ...
    > tag_tuple_type;

    typedef std::tuple<
        typename extract_tag<Tags>::value_type ...
    > value_tuple_type;
};

} // namespace mpxx

#endif // __MPXX_TAG_H__
