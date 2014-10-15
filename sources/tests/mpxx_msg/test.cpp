#define BOOST_TEST_MODULE mpxx_msg

#include <string>
#include <sstream>

#include <mpxx/unit_test.hpp>
#include <mpxx/mpxx.hpp>

MPXX_MSG(
    msg_type,
    (int, id)
    (std::string, str)
);

struct value_visitor
{
    template <typename T>
    void operator()(const T& v)
    {
        //std::cout << "VV: " << v << std::endl;
    }
};

BOOST_AUTO_TEST_CASE(mpxx_msg)
{
    msg_type m(42, "a message");

    BOOST_CHECK_MESSAGE(
        m.id == 42 && m.str == "a message",
        "access values by fields"
    );

    auto t = m(m.id_tag, m.str_tag);

    BOOST_CHECK_MESSAGE(
        std::get<0>(t) == 42 && std::get<1>(t) == "a message",
        "access values by tags"
    );

    m.str = "another message";

    BOOST_CHECK_MESSAGE(
        std::get<0>(t) == 42 && std::get<1>(t) == "another message",
        "reference updated"
    );

    m.for_each(value_visitor());
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
        rm.id == 42 && rm.str == "a message",
        "access values by fields"
    );
}

BOOST_AUTO_TEST_CASE(mpxx_msg_dump)
{
    msg_type m(42, "a message");

    std::ostringstream oss;
    oss << m;

    BOOST_CHECK_MESSAGE(
        oss.str() == "42,a message",
        "dump to stream"
    );
}
