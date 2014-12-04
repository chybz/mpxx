#ifndef __MPXX_MACROS_FIELD_H__
#define __MPXX_MACROS_FIELD_H__

#define MPXX_DEFINE_COMMON_FIELD(TYPE, NAME) \
typedef \
    BOOST_PP_CAT(NAME, _field)< \
        TYPE \
    > BOOST_PP_CAT(NAME, _field_type);

#define MPXX_INIT_2(I)
#define MPXX_INIT_3(I) = I

#define MPXX_INIT(SIZE) MPXX_INIT_ ## SIZE

#define MPXX_FIELD_STRUCT(TYPE, NAME, INIT) \
template <typename Type> \
struct TPL_CAT(NAME, _field) \
{ \
    typedef TPL_CAT(NAME, _field) this_type; \
    typedef Type type; \
    \
    struct TPL_CAT(NAME, _tag_type) : mpxx::tag_base {}; \
    \
    typedef TPL_CAT(NAME, _tag_type) tag_type; \
    \
    const tag_type TPL_CAT(NAME, _tag) = {}; \
    type NAME INIT; \
    \
    constexpr TPL_CAT(NAME, _field)() \
    {} \
    \
    TPL_CAT(NAME, _field)(const this_type& other) \
    : NAME(other.NAME) \
    {} \
    \
    TPL_CAT(NAME, _field)(this_type&& other) \
    : NAME(std::move(other.NAME)) \
    {} \
    \
    constexpr TPL_CAT(NAME, _field)(type&& v) \
    : NAME{std::forward<type>(v)} \
    {} \
    \
    this_type& operator=(const this_type& other) \
    { \
        NAME = other.NAME; \
        return *this; \
    } \
    \
    this_type& operator=(this_type&& other) \
    { \
        NAME = std::move(other.NAME); \
        return *this; \
    } \
    \
    constexpr static const char* name() \
    { return BOOST_PP_STRINGIZE(NAME); } \
    \
    type& value() & \
    { return NAME; } \
    \
    const type& value() const& \
    { return NAME; } \
    \
    type&& value() && \
    { return NAME; } \
};

#define MPXX_FIELD(T, ARG) \
    MPXX_FIELD_STRUCT( \
        TPL_ELEMENT(T, 0), \
        TPL_ELEMENT(T, 1), \
        TPL_CAT(MPXX_INIT_, TPL_SIZE(T))(TPL_ELEMENT(T, 2)) \
    )

#define MPXX_DEFINE_FIELD(R, COMMON, FIELD) \
    MPXX_DEFINE_FIELD_STRUCT( \
        BOOST_PP_TUPLE_ELEM(3, 0, FIELD), \
        BOOST_PP_TUPLE_ELEM(3, 1, FIELD), \
        BOOST_PP_TUPLE_ELEM(3, 2, FIELD), \
    ) \
    BOOST_PP_IF( \
        COMMON, \
        MPXX_DEFINE_COMMON_FIELD, \
        BOOST_PP_TUPLE_EAT(2) \
    )( \
        BOOST_PP_TUPLE_ELEM(3, 0, FIELD), \
        BOOST_PP_TUPLE_ELEM(3, 1, FIELD) \
    )

#define MPXX_MAKE_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(3, 1, FIELD), _field)<\
        BOOST_PP_TUPLE_ELEM(3, 0, FIELD) \
    >

#define MPXX_MAKE_EXT_FIELD(R, DATA, I, FIELD) \
    BOOST_PP_COMMA_IF(I) \
    BOOST_PP_CAT(BOOST_PP_EXPAND FIELD, _field_type)

#endif // __MPXX_MACROS_FIELD_H__
