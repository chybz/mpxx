#ifndef __MPXX_COMPILER_H__
#define __MPXX_COMPILER_H__

#ifdef UNIX
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif

#ifdef WIN32
#define likely(x)       x
#define unlikely(x)     x
#endif

#endif // __MPXX_COMPILER_H__
