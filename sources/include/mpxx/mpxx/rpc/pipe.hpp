#ifndef __MPXX_RPC_PIPE_H__
#define __MPXX_RPC_PIPE_H__

#include <nnxx/message>
#include <nnxx/socket>

namespace mpxx {
namespace rpc {

template <typename Derived, typename Messages...>
class pipe
{
public:
    pipe();

    virtual ~pipe()
    {}

private:
    /// CRTP interface
    Derived& derived()
    { return *static_cast<Derived* const>(this); }
    Derived const& derived() const
    { return *static_cast<Derived const*>(this); }

    nnxx::socket in_ { nnxx::SP, nnxx::PULL };
    nnxx::socket out_ { nnxx::SP, nnxx::PUSH };
};

} // namespace rpc
} // namespace mpxx

#endif // __MPXX_RPC_PIPE_H__
