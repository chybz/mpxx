#ifndef __MPXX_RPC_DECODER_H__
#define __MPXX_RPC_DECODER_H__

#include <type_traits>

#include <nnxx/message.h>

#include <mpxx/msgpack.hpp>

namespace mpxx {
namespace rpc {

template <typename Handler>
struct decoder_type
{
    using type = typename std::add_pointer<
        void(Handler&, const nnxx::message&)
    >::type;
}

template <typename Handler, typename Message>
auto make_decoder()
{
    return
        [](Handler& h, const nnxx::message& m) {
            msgpack::unpacked u;
            msgpack::unpack(
                &u,
                (const char*) m.data(), m.size()
            );

            Message msg;
            msgpack::object obj = u.get();
            obj.convert(&msg);

            h(msg);
        };
}

} // namespace rpc
} // namespace mpxx

#endif // __MPXX_RPC_DECODER_H__
