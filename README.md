mpxx
====

MessagePack interface and self-aware struct-like types for C++11.

This work is based on an original idea from [Joe Groff](http://duriansoftware.com/joe/Self-aware-struct-like-types-in-C++11.html).

Requirements
============

- C++11 compiler (tested with GCC 4.8.3 on Debian and Apple LLVM version 6.0 on OSX)
- Boost.Preprocessor
- [cpkg](https://github.com/chybz/cpkg.git) and [cbuild](https://github.com/chybz/cbuild.git) for building and packaging

Basic usage
===========

Define a MessagePack'able type `my_msg` with a `std::size_t` identifier and a
`std::string` label:

    MPXX_MSG(
        my_msg,
        (std::size_t, id)
        (std::string, label)
    )
