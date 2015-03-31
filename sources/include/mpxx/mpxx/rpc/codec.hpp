#ifndef __MPXX_RPC_CODEC_H__
#define __MPXX_RPC_CODEC_H__

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <cstring>
#include <sstream>

#include <nnxx/message.h>

#include <mpxx/msgpack.hpp>
#include <mpxx/compiler.hpp>
#include <mpxx/utils.hpp>
#include <mpxx/rpc/id_types.hpp>

namespace mpxx {
namespace rpc {

template <typename Handler>
struct decoder_type
{
    using type = typename std::add_pointer<
        void(Handler&, msgpack::unpacked& result)
    >::type;
};

template <typename Handler, typename Message>
auto make_decoder()
{
    return
        [](Handler& h, msgpack::unpacked& result) {
            Message msg;
            result.get().convert(&msg);
            h(msg);
        };
}

struct make_type_map
{
    using fwd_map_type = std::unordered_map<std::type_index, std::size_t>;
    using rev_map_type = std::unordered_map<std::size_t, std::type_index>;

    template <
        typename... Messages,
        std::size_t... Indices
    >
    static
    void
    make(
        fwd_map_type& fmap,
        rev_map_type& rmap,
        std::index_sequence<Indices...>
    )
    {
        fmap = {
            {
                std::type_index(typeid(Messages)),
                Indices
            }...
        };

        rmap = {
            {
                Indices,
                std::type_index(typeid(Messages))
            }...
        };
    }
};

template <protocol_id Id, typename Handler, typename... Messages>
class codec
{
public:
    using Indices = std::make_index_sequence<sizeof...(Messages)>;

    codec()
    : decoders_{ make_decoder<Handler, Messages>()... }
    { make_type_map::make<Messages...>(fwd_types_, rev_types_, Indices()); }

    virtual ~codec()
    {}

    template <typename Message>
    nnxx::message
    encode(const Message& m)
    {
        sbuf_.clear();

        msgpack::packer<msgpack::sbuffer> pk(sbuf_);
        pk.pack(Id);
        pk.pack(make_message_id(m));
        pk.pack(m);

        nnxx::message msg(sbuf_.size());
        std::memcpy(
            (void*) msg.data(),
            (const void*) sbuf_.data(), sbuf_.size()
        );

        return msg;
    }

    void decode(Handler& h, const nnxx::message& m)
    {
        upk_.reserve_buffer(m.size());
        std::memcpy(
            (void*) upk_.buffer(),
            (const void*) m.data(), m.size()
        );
        upk_.buffer_consumed(m.size());

        check_protocol_id();
        auto id = check_message_id();

        check_has_next("message data");
        decoders_[id](h, result_);
    }

private:
    using decoders = std::vector<
        typename decoder_type<Handler>::type
    >;
    using fwd_type_map = make_type_map::fwd_map_type;
    using rev_type_map = make_type_map::rev_map_type;

    template <typename Message>
    message_id
    make_message_id(const Message& m) const
    {
        auto index = std::type_index(typeid(Message));
        auto it = fwd_types_.find(index);

        if (unlikely(it == fwd_types_.end())) {
            throw std::runtime_error(
                mpxx::demangle_type_name(typeid(Message).name())
                + " is not in this protocol"
            );
        }

        return it->second;
    }

    void check_has_next(const std::string& what)
    {
        if (unlikely(!upk_.next(&result_))) {
            throw std::runtime_error(
                "not enough data to decode " + what
            );
        }
    }

    void check_protocol_id()
    {
        check_has_next("protocol id");

        protocol_id id;
        result_.get().convert(&id);

        if (unlikely(id != Id)) {
            std::ostringstream oss;

            oss
                << "invalid protocol id " << +id
                << " (mine is " << +Id << ")"
                ;

            throw std::runtime_error(oss.str());
        }
    }

    message_id check_message_id()
    {
        check_has_next("message id");

        message_id id;
        result_.get().convert(&id);

        if (unlikely(id >= decoders_.size())) {
            std::ostringstream oss;

            oss
                << "invalid message id " << +id
                << " (max is " << decoders_.size() << ")"
                ;

            throw std::runtime_error(oss.str());
        }

        return id;
    }

    decoders decoders_;
    fwd_type_map fwd_types_;
    rev_type_map rev_types_;
    msgpack::sbuffer sbuf_;
    msgpack::unpacker upk_;
    msgpack::unpacked result_;
};

} // namespace rpc
} // namespace mpxx

#endif // __MPXX_RPC_CODEC_H__
