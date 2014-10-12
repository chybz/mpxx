#ifndef __MPXX_DEFINE_H__
#define __MPXX_DEFINE_H__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>

namespace mpxx {
namespace detail {

template <typename T>
struct arg_type;

template <typename T, typename U>
struct arg_type<T(U)>
{ typedef U type; };

} // namespace detail
} // namespace mpxx

#define MPXX_FILLER_0(X, Y) \
    ((X, Y)) MPXX_FILLER_1
#define MPXX_FILLER_1(X, Y) \
    ((X, Y)) MPXX_FILLER_0
#define MPXX_FILLER_0_END
#define MPXX_FILLER_1_END

#define MPXX_DEFINE_TAG(R, DATA, TUPLE) \
struct BOOST_PP_TUPLE_ELEM(2, 0, TUPLE) {};

#define MPXX_DEFINE_IDX(R, DATA, TUPLE) \
static constexpr std::size_t BOOST_PP_TUPLE_ELEM(2, 0, TUPLE) = R - 2;

#define MPXX_DEFINE_MPXX_TAG(R, DATA, TUPLE) \
    (mpxx::detail::arg_type<void(mpxx::tag< \
        BOOST_PP_CAT(tags::, BOOST_PP_TUPLE_ELEM(2, 0, TUPLE)), \
        BOOST_PP_TUPLE_ELEM(2, 1, TUPLE) \
    >)>::type)

#define MPXX_DEFINE_MSG(NAME, TUPLES) \
namespace tags { \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_TAG, ~, TUPLES) \
} \
namespace idx { \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_IDX, ~, TUPLES) \
} \
\
typedef mpxx::msg< \
    BOOST_PP_SEQ_ENUM( \
        BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_MPXX_TAG, ~, TUPLES) \
    ) \
> NAME;

#define MPXX_MSG(NAME, TUPLES) \
    MPXX_DEFINE_MSG( \
        NAME, \
        BOOST_PP_CAT(MPXX_FILLER_0 TUPLES, _END) \
    )

#endif // __MPXX_DEFINE_H__
