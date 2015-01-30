#ifndef __MPXX_RPC_PROTOCOL_H__
#define __MPXX_RPC_PROTOCOL_H__

#include <stdint.h>

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <type_traits>

#include <nnxx/message.h>

#include <mpxx/mpxx.hpp>
#include <mpxx/rpc/encoder.hpp>
#include <mpxx/rpc/decoder.hpp>

namespace mpxx {
namespace rpc {

struct make_type_map
{
    using map_type = std::unordered_map<std::type_index, std::size_t>;

    template <
        typename... Messages,
        std::size_t... Indices
    >
    static
    void
    make(map_type& map, std::index_sequence<Indices...>)
    {
        map = {
            {
                std::type_index(typeid(Messages)),
                Indices
            }...
        };
    }
};

template <uint8_t Id, typename Handler, typename... Messages>
class protocol
{
public:
    using Indices = std::make_index_sequence<sizeof...(Messages)>;

    protocol(Handler& h)
    : h_(h),
    decoder_map_{
        {
            std::type_index(typeid(Messages)),
            make_decoder<Handler, Messages>()
        }...
    }
    {
        make_type_map::make<Messages...>(type_map_, Indices());
    }

    virtual ~protocol()
    {}

    template <typename Message>
    nnxx::message
    encode(const Message& m) const
    { return mpxx::rpc::encode(m); }

    void decode(const nnxx::message& m) const
    {

    }

    void dump() const
    {
        for (const auto& p : type_map_) {
            std::cout
                << "id:" << p.first.hash_code()
                << " name:" << p.second
                << std::endl;
        }
    }

private:
    using msg_type_map = std::unordered_map<std::type_index, std::size_t>;
    using msg_decoder_map = std::unordered_map<
        std::type_index,
        typename decoder_type<Handler>::type
    >;

    Handler& h_;
    msg_type_map type_map_;
    msg_decoder_map decoder_map_;
};

} // namespace rpc
} // namespace mpxx

#endif // __MPXX_RPC_PROTOCOL_H__
