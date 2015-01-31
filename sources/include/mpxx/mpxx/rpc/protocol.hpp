#ifndef __MPXX_RPC_PROTOCOL_H__
#define __MPXX_RPC_PROTOCOL_H__

#include <iostream>

#include <utility>
#include <type_traits>

#include <nnxx/message.h>

#include <mpxx/mpxx.hpp>
#include <mpxx/rpc/id_types.hpp>
#include <mpxx/rpc/codec.hpp>

namespace mpxx {
namespace rpc {

template <protocol_id Id, typename Handler, typename... Messages>
class protocol
{
public:
    protocol(Handler& h)
    : h_(h),
    codec_()
    {}

    virtual ~protocol()
    {}

    template <typename Message>
    nnxx::message
    encode(const Message& m)
    { return codec_.encode(m); }

    void decode(const nnxx::message& m)
    { codec_.decode(h_, m); }

private:
    using codec_type = codec<Id, Handler, Messages...>;

    Handler& h_;
    codec_type codec_;
};

} // namespace rpc
} // namespace mpxx

#endif // __MPXX_RPC_PROTOCOL_H__
