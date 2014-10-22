#ifndef __MPXX_MACROS_FIELDS_H__
#define __MPXX_MACROS_FIELDS_H__

#include <boost/preprocessor/seq.hpp>

#include <mpxx/macros/field.hpp>

#define MPXX_DEFINE_FIELDS(FIELDS) \
    BOOST_PP_SEQ_FOR_EACH(MPXX_DEFINE_FIELD, 1, FIELDS)

#endif // __MPXX_MACROS_FIELDS_H__
