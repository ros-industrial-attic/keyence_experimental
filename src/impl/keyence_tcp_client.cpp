#include "keyence/impl/keyence_tcp_client.h"

keyence::TcpClient::TcpClient(const std::string& host, const std::string& port)
  : sock_(host, port, LIBSOCKET_IPv4, 0)
{}

int keyence::TcpClient::send(const void* data, size_t size)
{
  return sock_.snd(data, size);
} 

int keyence::TcpClient::recv(void* data, size_t size)
{
  return sock_.rcv(data, size);
}