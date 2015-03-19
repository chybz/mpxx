#define BOOST_TEST_MODULE mpxx_msg

#include <string>
#include <sstream>
#include <type_traits>

#include <mpxx/unit_test.hpp>
#include <mpxx/mpxx.hpp>

// Standalone message with internal fields
MPXX_MSG(
    msg_type,
    (int, id),
    (std::string, str)
);

// Standalone message with internal fields and default values
MPXX_MSG(
    msg_def_type,
    (int, id, 0),
    (std::string, str, "")
);

namespace fields {

// Define some field types in a common namespace
MPXX_FIELDS(
    (std::size_t, counter),
    (bool, valid),
    (std::string, label),
    (std::string, label2),
    (double, avg)
);

} // namespace fields

// Define a struct using predefined field types
MPXX_STRUCT(
    mstruct1,
    (fields::valid),
    (fields::avg)
);

MPXX_STRUCT(
    mstruct4,
    (fields::valid),
    (fields::avg),
	(fields::label)
);

namespace structs {

struct foo {
    // Define structss using predefined field types
    MPXX_STRUCT(
        mstruct2,
        (::fields::counter),
        (::fields::avg)
    );

    MPXX_STRUCT(
        mstruct3,
        (fields::counter),
        (fields::valid),
        (fields::label2),
        (fields::avg),
        (fields::label)
    );

    mstruct2 m2;
    mstruct3 m3;
};

} // namespace structs

// Define messages using predefined field types
MPXX_MSG(
    msg1,
    (fields::valid),
    (fields::avg)
);

MPXX_MSG(
    msg2,
    (::fields::counter),
    (::fields::avg)
);

MPXX_MSG(
    msg3,
    (fields::counter),
    (fields::valid),
    (fields::label2),
    (fields::avg),
    (fields::label)
);

struct value_visitor
{
    template <typename T>
    void operator()(const T& v)
    {
        //std::cout << "VV: " << v << std::endl;
    }
};

struct field_visitor
{
	template<typename T>
	void operator() (T& v, std::size_t i)
	{
		std::string label = v.name();

		if (i == 2) {
			BOOST_CHECK_MESSAGE(
				label == "label",
				"Check label name from pos"
			);
		}

		//std::cout << v.name() << " = " << v.value() << std::endl;
	}
};

BOOST_AUTO_TEST_CASE(mpxx_mstruct)
{
    mstruct1 m(false, 42.42);

    BOOST_CHECK_MESSAGE(
        m.valid == false && m.avg == 42.42,
        "mstruct access values by fields"
    );

    // Grab field values in a tuple, using field tags
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

    mstruct1 m2(false, 84.84);

    m = m2;

    BOOST_CHECK_MESSAGE(
        m.valid == false && m.avg == 84.84,
        "mstruct assign"
    );

    mstruct1 mc(m);

    BOOST_CHECK_MESSAGE(
        mc.valid == false && mc.avg == 84.84,
        "mstruct copy constructor"
    );

    mc.valid = true;
    mc.avg = 56.23;

    m = std::move(mc);

    BOOST_CHECK_MESSAGE(
        m.valid == true && m.avg == 56.23,
        "mstruct move assign"
    );
}

BOOST_AUTO_TEST_CASE(mstruct_check_foreach_field)
{
	mstruct4 m(true, 0.0, "toto");
	m.for_each<mpxx::field_pos_visit>(field_visitor());
}

BOOST_AUTO_TEST_CASE(mpxx_msg)
{
    msg_type m(42, "a message");

    BOOST_CHECK_MESSAGE(
        m.describe() == "id:sint32,str:string",
        "msg description"
    );

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

    // Assign message values via a tuple
    m = std::make_tuple(84, "foo");

    BOOST_CHECK_MESSAGE(
        m.id == 84 && m.str == "foo",
        "msg assign via tuple"
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
        rm.id == 42 && rm.str == "a message",
        "access values by fields"
    );

    BOOST_CHECK_MESSAGE(
        m == rm,
        "messages are equal"
    );

    rm.id++;

    BOOST_CHECK_MESSAGE(
        m != rm,
        "messages are different"
    );
}

BOOST_AUTO_TEST_CASE(mpxx_struct_intersect)
{
    // Check various use cases
    mstruct1 m1(true, 42.42);
    structs::foo f;

    f.m2 = structs::foo::mstruct2(1234, 42.43);
    f.m3 = structs::foo::mstruct3(4567, false, "a string", 84.84, "a name");

    m1 = f.m2;

    BOOST_CHECK_MESSAGE(
        m1.avg == f.m2.avg && m1.avg == 42.43,
        "common field updated"
    );

    BOOST_CHECK_MESSAGE(
        m1.valid == true,
        "other field untouched"
    );

    f.m3 = m1;

    BOOST_CHECK_MESSAGE(
        f.m3.valid && f.m3.avg == 42.43,
        "common fields updated"
    );

    BOOST_CHECK_MESSAGE(
        f.m3.counter == 4567 && f.m3.label == "a name",
        "other fields untouched"
    );
}

BOOST_AUTO_TEST_CASE(mpxx_msg_intersect)
{
    msg1 m1(true, 42.42);
    msg2 m2(1234, 42.43);
    msg3 m3(4567, false, "a string", 84.84, "a name");

    m1 = m2;

    BOOST_CHECK_MESSAGE(
        m1.avg == m2.avg && m1.avg == 42.43,
        "common field updated"
    );

    BOOST_CHECK_MESSAGE(
        m1.valid == true,
        "other field untouched"
    );

    m3 = m1;

    BOOST_CHECK_MESSAGE(
        m3.valid && m3.avg == 42.43,
        "common fields updated"
    );

    BOOST_CHECK_MESSAGE(
        m3.counter == 4567 && m3.label == "a name",
        "other fields untouched"
    );
}

BOOST_AUTO_TEST_CASE(mpxx_msg_no_intersect)
{
    msg_type m(42, "a message");
    msg1 m1(true, 42.42);

    m = m1;

    BOOST_CHECK_MESSAGE(
        m.id == 42 && m.str == "a message"
        &&
        m1.valid == true && m1.avg == 42.42
        ,
        "no common fields"
    );
}

BOOST_AUTO_TEST_CASE(mpxx_msg_update_non_default)
{
    msg_def_type m1;
    m1.id = 42;

    msg_def_type m2;
    m2.str = "junk";

    m1 |= m2;

    BOOST_CHECK_MESSAGE(
        m1.id == 42 && m1.str == "junk",
        "non-default update"
    );
}
