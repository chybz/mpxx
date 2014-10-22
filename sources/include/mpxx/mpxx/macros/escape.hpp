#ifndef __MPXX_MACROS_ESCAPE_H__
#define __MPXX_MACROS_ESCAPE_H__

// Preprocessor boilerplate stolen from boost::fusion::adapt_struct
#define MPXX_TESC_0(X, Y) \
    ((X, Y)) MPXX_TESC_1
#define MPXX_TESC_1(X, Y) \
    ((X, Y)) MPXX_TESC_0
#define MPXX_TESC_0_END
#define MPXX_TESC_1_END

#define MPXX_ESC_0(X) \
    ((X)) MPXX_ESC_1
#define MPXX_ESC_1(X) \
    ((X)) MPXX_ESC_0
#define MPXX_ESC_0_END
#define MPXX_ESC_1_END

#endif // __MPXX_MACROS_ESCAPE_H__
