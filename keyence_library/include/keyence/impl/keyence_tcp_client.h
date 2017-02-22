#ifndef KEYENCE_TCP_CLIENT_H
#define KEYENCE_TCP_CLIENT_H

#include "keyence_client.h"
#include "inetclientstream.hpp"

namespace keyence
{

/**
 * @brief Tcp/IP implementation of the Keyence Client using libsocket++
 *
 * Throws 'keyence::KeyenceException' upon error
 */
class TcpClient : public Client
{
public:
  TcpClient(const std::string& host, const std::string& port);

  int send(const void* data, std::size_t size) override;
  int recv(void* data, std::size_t size) override;

protected:
  libsocket::inet_stream sock_;
};

}

#endif
