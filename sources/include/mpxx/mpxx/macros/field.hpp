#ifndef __MPXX_MACROS_FIELD_H__
#define __MPXX_MACROS_FIELD_H__

#define MPXX_TO_STRING(WHAT) #WHAT

//
// Name of field struct
//
// common fields: MPXX_FIELD_NAME((NAME)) --> NAME_field
// internal fields: MPXX_FIELD_NAME((CLASS, NAME)) --> CLASS_NAME_field
//
#define MPXX_FIELD_NAME_1(NAME) \
    TPL_CAT(NAME, _field)
#define MPXX_FIELD_NAME_2(CLASS, NAME) \
    TPL_CAT(CLASS, TPL_CAT(_, MPXX_FIELD_NAME_1(NAME)))

#define MPXX_FIELD_NAME(...) \
    TPL_CAT(MPXX_FIELD_NAME_, TPL_SIZE((__VA_ARGS__)))(__VA_ARGS__)

//
// Field default initialization
//
// MPXX_FIELD_INIT() -->
// MPXX_FIELD_INIT(expr) --> = expr
//
#define MPXX_FIELD_INIT_0(I)
#define MPXX_FIELD_INIT_1(I) = I

#define MPXX_FIELD_INIT(INIT) \
    TPL_CAT(MPXX_FIELD_INIT_, TPL_SIZE_L(INIT))(INIT)

//
// Field struct definition
//
// T    : (NAME), (CLASS, NAME)
// TYPE : type of field (std::size_t, float, ...)
// NAME : name of field (C++ member)
// INIT : optional field default value
//
#define MPXX_FIELD_STRUCT(T, TYPE, NAME, INIT) \
template <typename Type> \
struct MPXX_FIELD_NAME(TPL_TO_LIST(T)) \
{ \
    typedef MPXX_FIELD_NAME(TPL_TO_LIST(T)) this_type; \
    typedef Type type; \
    \
    struct TPL_CAT(NAME, _tag_type) : mpxx::tag_base {}; \
    \
    typedef TPL_CAT(NAME, _tag_type) tag_type; \
    \
    const tag_type TPL_CAT(NAME, _tag) = {}; \
    type NAME MPXX_FIELD_INIT(INIT); \
    \
    constexpr MPXX_FIELD_NAME(TPL_TO_LIST(T))() \
    {} \
    \
    MPXX_FIELD_NAME(TPL_TO_LIST(T))(const this_type& other) \
    : NAME(other.NAME) \
    {} \
    \
    MPXX_FIELD_NAME(TPL_TO_LIST(T))(this_type&& other) \
    : NAME(std::move(other.NAME)) \
    {} \
    \
    constexpr MPXX_FIELD_NAME(TPL_TO_LIST(T))(type&& v) \
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
    { return MPXX_TO_STRING(NAME); } \
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

//
// Common field global type definition
//
#define MPXX_COMMON_FIELD_0(TYPE, NAME) \
typedef \
    TPL_CAT(NAME, _field)< \
        TYPE \
    > TPL_CAT(NAME, _field_type);

#define MPXX_COMMON_FIELD_1(TYPE, NAME)

#define MPXX_COMMON_FIELD(TYPE, NAME, COMMON) \
    TPL_CAT(MPXX_COMMON_FIELD_, TPL_SIZE_L(COMMON))(TYPE, NAME)

//
// Base name of field struct
//
#define MPXX_FIELD_BASE_NAME_0(BASE, NAME) (NAME)
#define MPXX_FIELD_BASE_NAME_1(BASE, NAME) (BASE, NAME)

#define MPXX_FIELD_BASE_NAME(NAME, COMMON) \
    TPL_CAT(MPXX_FIELD_BASE_NAME_, TPL_SIZE_L(COMMON))( \
        COMMON, \
        NAME \
    )

//
// Field definition
//
// T      : (C++ type, name[, default value])
// COMMON : empty for common field, struct name otherwise
//
#define MPXX_FIELD(T, COMMON) \
    MPXX_FIELD_STRUCT( \
        MPXX_FIELD_BASE_NAME( \
            TPL_ELEMENT(T, 1), \
            COMMON \
        ), \
        TPL_ELEMENT(T, 0), \
        TPL_ELEMENT(T, 1), \
        TPL_ELEMENT(T, 2) \
    ) \
    MPXX_COMMON_FIELD( \
        TPL_ELEMENT(T, 0), \
        TPL_ELEMENT(T, 1), \
        COMMON \
    )

//
// Field type when declaring a struct or message
//
#define MPXX_FIELD_TYPE(T, CLASS) \
    MPXX_FIELD_NAME(CLASS, TPL_ELEMENT(T, 1))< \
        TPL_ELEMENT(T, 0) \
    >

//
// struct or message field definition
//
#define MPXX_STRUCT_FIELD_1(T, NAME)
#define MPXX_STRUCT_FIELD_2(T, NAME) \
    MPXX_FIELD(T, NAME)
#define MPXX_STRUCT_FIELD_3(T, NAME) \
    MPXX_FIELD(T, NAME)

#define MPXX_STRUCT_FIELD(T, NAME) \
    TPL_CAT(MPXX_STRUCT_FIELD_, TPL_SIZE(T))(T, NAME)

//
// struct or message field type
//
#define MPXX_STRUCT_FIELD_TYPE_1(T, NAME) \
    TPL_CAT(TPL_ELEMENT(T, 0), _field_type)
#define MPXX_STRUCT_FIELD_TYPE_2(T, NAME) \
    MPXX_FIELD_TYPE(T, NAME)
#define MPXX_STRUCT_FIELD_TYPE_3(T, NAME) \
    MPXX_FIELD_TYPE(T, NAME)

#define MPXX_STRUCT_FIELD_TYPE(T, NAME) \
    TPL_CAT(MPXX_STRUCT_FIELD_TYPE_, TPL_SIZE(T))(T, NAME)

#endif // __MPXX_MACROS_FIELD_H__
