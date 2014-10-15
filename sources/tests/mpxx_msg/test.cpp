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
        std::cout << "VV: " << v << std::endl;
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

// BOOST_AUTO_TEST_CASE(mpxx_msg_pack_unpack)
// {
//     msg_type m(42, "a message");

//     msgpack::sbuffer sbuf;
//     msgpack::pack(sbuf, m);

//     msgpack::unpacked u;
//     msgpack::unpack(&u, sbuf.data(), sbuf.size());

//     msg_type rm;
//     msgpack::object obj = u.get();
//     obj.convert(&rm);

//     BOOST_CHECK_MESSAGE(
//         rm.get<0>() == 42 && rm.get<1>() == "a message",
//         "access values by index"
//     );

//     BOOST_CHECK_MESSAGE(
//         rm.get<tags::msg_id>() == 42 && rm.get<tags::msg_str>() == "a message",
//         "access values by tag"
//     );
// }

// BOOST_AUTO_TEST_CASE(mpxx_msg_dump)
// {
//     msg_type m(42, "a message");

//     std::ostringstream oss;
//     oss << m;

//     BOOST_CHECK_MESSAGE(
//         oss.str() == "42,a message",
//         "dump to stream"
//     );
// }

// BOOST_AUTO_TEST_CASE(mpxx_msg_access)
// {
//     msg_type m(42, "a message");

//     m.set<0, 1>(44, "junk");

//     BOOST_CHECK_MESSAGE(
//         m[tags::msg_id()] == 44 && m[tags::msg_str()] == "junk",
//         "compound assignment via set<> and indices"
//     );

//     m.set<tags::msg_id, tags::msg_str>(45, "foo");

//     BOOST_CHECK_MESSAGE(
//         m[tags::msg_id()] == 45 && m[tags::msg_str()] == "foo",
//         "compound assignment via set<> and tags"
//     );

//     m[tags::msg_id()] = 43;

//     BOOST_CHECK_MESSAGE(
//         m[tags::msg_id()] == 43,
//         "assignment via operator[]"
//     );

//     m.set<1>("another message");

//     BOOST_CHECK_MESSAGE(
//         m[tags::msg_str()] == "another message",
//         "assignment via set<>"
//     );

//     auto t1 = m.get<tags::msg_id, tags::msg_str>();
//     auto t2 = m(tags::msg_id(), tags::msg_str());

//     BOOST_CHECK_MESSAGE(
//         std::get<0>(t1) == 43 && std::get<1>(t1) == "another message",
//         "message value tuple"
//     );

//     m(tags::msg_id(), tags::msg_str()) = std::make_tuple(1234, "whoa");

//     BOOST_CHECK_MESSAGE(
//         m[tags::msg_id()] == 1234 && m[tags::msg_str()] == "whoa",
//         "compound assignment via operator[]"
//     );

//     BOOST_CHECK_MESSAGE(
//         std::get<0>(t1) == 1234 && std::get<1>(t1) == "whoa",
//         "message value tuple updated"
//     );

//     BOOST_CHECK_MESSAGE(
//         std::get<0>(t2) == 1234 && std::get<1>(t2) == "whoa",
//         "message value tuple updated"
//     );
// }
