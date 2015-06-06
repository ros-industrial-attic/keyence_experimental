#ifndef KEYENCE_TCP_CLIENT_H
#define KEYENCE_TCP_CLIENT_H

#include "keyence_client.h"
#include "libsocket/inetclientstream.hpp"

namespace keyence
{

class TcpClient : public Client
{
public:
  TcpClient(const std::string& host, const std::string& port);

  virtual int send(const void* data, std::size_t size); // override
  virtual int recv(void* data, std::size_t size); // override

protected:
  libsocket::inet_stream sock_;
};

}

#endif