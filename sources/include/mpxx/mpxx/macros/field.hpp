#ifndef __MPXX_MACROS_FIELD_H__
#define __MPXX_MACROS_FIELD_H__

#define MPXX_DEFINE_COMMON_FIELD(TYPE, NAME) \
typedef \
    BOOST_PP_CAT(NAME, _field)< \
        TYPE \
    > BOOST_PP_CAT(NAME, _field_type);

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

#define MPXX_INIT_2(I)
#define MPXX_INIT_3(I) = I
#define MPXX_INIT(SIZE) MPXX_INIT_ ## SIZE

#define MPXX_COMMON_FIELD_0(NAME)
#define MPXX_COMMON_FIELD_1(NAME)

#define MPXX_FIELD(T, NAME) \
    MPXX_FIELD_STRUCT( \
        TPL_ELEMENT(T, 0), \
        TPL_ELEMENT(T, 1), \
        TPL_CAT(MPXX_INIT_, TPL_SIZE(T))(TPL_ELEMENT(T, 2)) \
    ) \
    TPL_CAT(MPXX_COMMON_FIELD_, TPL_SIZE_L(NAME))(NAME)

#define MPXX_FIELD_TYPE(T, ARG) \
    TPL_CAT(TPL_ELEMENT(T, 1), _field)<\
        TPL_ELEMENT(T, 0) \
    >

#endif // __MPXX_MACROS_FIELD_H__
