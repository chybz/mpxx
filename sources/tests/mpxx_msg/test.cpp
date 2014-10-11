#define BOOST_TEST_MODULE mpxx_msg

#include <string>

#include <mpxx/unit_test.hpp>
#include <mpxx/mpxx.hpp>

struct msg_id {};
struct msg_str {};

typedef mpxx::msg<
    mpxx::tag<msg_id, int>,
    mpxx::tag<msg_str, std::string>
> msg_type;

BOOST_AUTO_TEST_CASE(mpxx_msg)
{
    msg_type m(42, "a message");

    BOOST_CHECK_MESSAGE(
        m.get<0>() == 42 && m.get<1>() == "a message",
        "access values by index"
    );

    BOOST_CHECK_MESSAGE(
        m.get<msg_id>() == 42 && m.get<msg_str>() == "a message",
        "access values by tag"
    );
}

BOOST_AUTO_TEST_CASE(mpxx_msg_pack_unpack)
{
    msg_type m(42, "a message");

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, m);

    msgpack::unpacked u;
    msgpack::unpack(&u, sbuf.data(), sbuf.size());

    msg_type rm;
    msgpack::object obj = u.get();
    obj.convert(&rm);

    BOOST_CHECK_MESSAGE(
        rm.get<0>() == 42 && rm.get<1>() == "a message",
        "access values by index"
    );

    BOOST_CHECK_MESSAGE(
        rm.get<msg_id>() == 42 && rm.get<msg_str>() == "a message",
        "access values by tag"
    );
}
