#define BOOST_TEST_MODULE mpxx_rpc

#include <iostream>
#include <string>
#include <vector>

#include <mpxx/unit_test.hpp>
#include <mpxx/rpc/protocol.hpp>

struct handler
{
    template <typename Message>
    void operator()(const Message& m)
    {
        std::cout
            << "got message: " << m
            << std::endl
            ;
    }
};

BOOST_AUTO_TEST_CASE(mpxx_rpc_protocol)
{
    handler h;

    mpxx::rpc::protocol<
        42,
        handler,
        float, std::string
    > proto(h);

    auto msg = proto.encode((float) 42.42);

    proto.decode(msg);
}
