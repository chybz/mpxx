#ifndef __MPXX_RPC_PIPE_H__
#define __MPXX_RPC_PIPE_H__

#include <nnxx/message>
#include <nnxx/socket>

#include <mpxx/rpc/protocol.hpp>

namespace mpxx {
namespace rpc {

template <typename Derived, protocol_id Id, typename Messages...>
class pipe
{
public:
    using protocol_type = protocol<Id, Derived, Messages...>;

    pipe()
    : protocol_(derived())
    {}

    virtual ~pipe()
    {}

private:
    /// CRTP interface
    Derived& derived()
    { return *static_cast<Derived* const>(this); }
    Derived const& derived() const
    { return *static_cast<Derived const*>(this); }

    protocol_type protocol_;
    nnxx::socket in_ { nnxx::SP, nnxx::PULL };
    nnxx::socket out_ { nnxx::SP, nnxx::PUSH };
};

} // namespace rpc
} // namespace mpxx

#endif // __MPXX_RPC_PIPE_H__
