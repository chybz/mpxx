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

MPXX_FIELDS(
    (std::size_t, counter)
    (bool, valid)
    (std::string, label)
    (double, avg)
);

MPXX_STRUCT_EXT_FIELDS(
    mstruct1,
    (valid)
    (avg)
);

MPXX_STRUCT_EXT_FIELDS(
    mstruct2,
    (counter)
    (avg)
);

MPXX_STRUCT_EXT_FIELDS(
    mstruct3,
    (counter)
    (valid)
    (avg)
    (label)
);

struct value_visitor
{
    template <typename T>
    void operator()(const T& v)
    {
        //std::cout << "VV: " << v << std::endl;
    }
};

BOOST_AUTO_TEST_CASE(mpxx_mstruct)
{
    mstruct1 m(false, 42.42);

    BOOST_CHECK_MESSAGE(
        m.valid == false && m.avg == 42.42,
        "mstruct access values by fields"
    );

    auto t = m(m.valid_tag, m.avg_tag);

    BOOST_CHECK_MESSAGE(
        std::get<0>(t) == false && std::get<1>(t) == 42.42,
        "mstruct access values by tags"
    );

    m.valid = true;

    BOOST_CHECK_MESSAGE(
        std::get<0>(t) == true && std::get<1>(t) == 42.42,
        "mstruct reference updated"
    );

    m.for_each(value_visitor());
}

BOOST_AUTO_TEST_CASE(mpxx_msg)
{
    msg_type m(42, "a message");

    BOOST_CHECK_MESSAGE(
        m.id == 42 && m.str == "a message",
        "msg access values by fields"
    );

    auto t = m(m.id_tag, m.str_tag);

    BOOST_CHECK_MESSAGE(
        std::get<0>(t) == 42 && std::get<1>(t) == "a message",
        "msg access values by tags"
    );

    m.str = "another message";

    BOOST_CHECK_MESSAGE(
        std::get<0>(t) == 42 && std::get<1>(t) == "another message",
        "msg reference updated"
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

BOOST_AUTO_TEST_CASE(mpxx_intersect)
{
    mstruct1 m1(true, 42.42);
    mstruct2 m2(1234, 42.43);
    mstruct3 m3(4567, false, 84.84, "a name");

    m1 << m2;

    BOOST_CHECK_MESSAGE(
        m1.avg == m2.avg && m1.avg == 42.43,
        "common field updated"
    );

    BOOST_CHECK_MESSAGE(
        m1.valid == true,
        "other field untouched"
    );

    m3 << m1;

    BOOST_CHECK_MESSAGE(
        m3.valid && m3.avg == 42.43,
        "common fields updated"
    );

    BOOST_CHECK_MESSAGE(
        m3.counter == 4567 && m3.label == "a name",
        "other fields untouched"
    );
}
