#ifndef __MPXX_RPC_ENCODER_H__
#define __MPXX_RPC_ENCODER_H__

#include <type_traits>

#include <mpxx/msgpack.hpp>

namespace mpxx {
namespace rpc {

template <typename Message>
auto encode(const Message& m)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, m);

    return
        nnxx::make_message_from(
            (void* ) sbuf.data(),
            sbuf.size()
        );
}

} // namespace rpc
} // namespace mpxx

#endif // __MPXX_RPC_ENCODER_H__
