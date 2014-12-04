#ifndef __MPXX_MACROS_ESCAPE_H__
#define __MPXX_MACROS_ESCAPE_H__

// Preprocessor boilerplate stolen from boost::fusion::adapt_struct
#define MPXX_TESC2_0(X, Y) \
    ((X, Y, )) MPXX_TESC2_1
#define MPXX_TESC2_1(X, Y) \
    ((X, Y, )) MPXX_TESC2_0
#define MPXX_TESC2_0_END
#define MPXX_TESC2_1_END

#define MPXX_TESC3_0(X, Y, Z) \
    ((X, Y, = Z)) MPXX_TESC3_1
#define MPXX_TESC3_1(X, Y, Z) \
    ((X, Y, = Z)) MPXX_TESC3_0
#define MPXX_TESC3_0_END
#define MPXX_TESC3_1_END

#define MPXX_GET_ESC(_1, _2, _3, NAME, ...) NAME

#define MPXX_ESC(...) \
    MPXX_GET_ESC(__VA_ARGS__, MPXX_TESC3_0, MPXX_TESC2_0)(__VA_ARGS__)

#define MPXX_ESC_0(X) \
    ((X)) MPXX_ESC_1
#define MPXX_ESC_1(X) \
    ((X)) MPXX_ESC_0
#define MPXX_ESC_0_END
#define MPXX_ESC_1_END

#endif // __MPXX_MACROS_ESCAPE_H__
