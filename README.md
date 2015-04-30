mpxx
====

MessagePack interface and self-aware struct-like types for C++11.

This work is based on an original idea from [Joe Groff](http://duriansoftware.com/joe/Self-aware-struct-like-types-in-C++11.html).

Requirements
============

- C++11 compiler (tested with GCC 4.8.3 on Debian and Apple LLVM version 6.0 on OSX)
- MessagePack
- [cpkg](https://github.com/chybz/cpkg.git) and [cbuild](https://github.com/chybz/cbuild.git) for building and packaging

Basic usage
===========

Define a MessagePack'able type `my_msg` with a `std::size_t` identifier and a
`std::string` label:

    #include <iostream>

    #include <mpxx/mpxx.hpp>

    MPXX_MSG(
        my_msg,
        (std::size_t, id)
        (std::string, label)
    )

    int main(int argc, char** argv)
    {
        my_msg m(42, "a message");

        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, m);

        msgpack::unpacked u;
        msgpack::unpack(&u, sbuf.data(), sbuf.size());

        my_msg rm;
        msgpack::object obj = u.get();
        obj.convert(&rm);

        std::cout
            << m.id << " " << rm.id << "\n"
            << m.label << " " << rm.label
            << std::endl;

        return 0;
    }
