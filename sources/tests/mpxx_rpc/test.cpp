#define BOOST_TEST_MODULE mpxx_rpc

#include <string>
#include <vector>

#include <mpxx/unit_test.hpp>
#include <mpxx/rpc/protocol.hpp>

struct handler
{
    template <typename Message>
    void operator()(const Message& m)
    {}
};

BOOST_AUTO_TEST_CASE(mpxx_rpc_protocol)
{
    handler h;

    mpxx::rpc::protocol<
        handler,
        float, std::vector<std::string>
    > proto(h);

    proto.dump();
}
