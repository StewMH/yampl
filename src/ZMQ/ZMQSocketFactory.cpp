#include <zmq.hpp>

#include "ZMQ/ZMQSocketFactory.h"
#include "ZMQ/ZMQSocket.h"

namespace YAMPL{
namespace ZMQ{

SocketFactory::SocketFactory() : m_context(new zmq::context_t(1)){
}

SocketFactory::~SocketFactory(){
  delete m_context;
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return new ClientSocket(channel, m_context, semantics, deallocator);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return new ServerSocket(channel, m_context, semantics, deallocator);
}

}
}
