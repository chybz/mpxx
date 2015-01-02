#define BOOST_TEST_MODULE mpxx_enum

#include <string>
#include <sstream>
#include <type_traits>

#include <mpxx/unit_test.hpp>
#include <mpxx/mpxx.hpp>

enum class color
{
    red,
    green,
    blue
};

MPXX_MSG(
    msg_type,
    (int, i, 42),
    (color, c, color::green)
);

BOOST_AUTO_TEST_CASE(mpxx_enum_msg_pack_unpack)
{
    msg_type m;

    BOOST_CHECK_MESSAGE(
        m.i == 42 && m.c == color::green,
        "default values"
    );

    m.c = color::red;

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, m);

    msgpack::unpacked u;
    msgpack::unpack(&u, sbuf.data(), sbuf.size());

    msg_type rm;
    msgpack::object obj = u.get();
    obj.convert(&rm);

    BOOST_CHECK_MESSAGE(
        rm.i == 42 && m.c == color::red,
        "default values"
    );

    BOOST_CHECK_MESSAGE(
        m == rm,
        "messages are equal"
    );

    rm.c = color::blue;

    BOOST_CHECK_MESSAGE(
        m != rm,
        "messages are different"
    );
}
