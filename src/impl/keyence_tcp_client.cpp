#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"

keyence::TcpClient::TcpClient(const std::string& host, const std::string& port)
  try: sock_(host, port, LIBSOCKET_IPv4, 0)
{

}
catch (const libsocket::socket_exception& ex)
{
  throw KeyenceException(ex.mesg);
}


int keyence::TcpClient::send(const void* data, size_t size)
{
  unsigned total_sent = 0;
  const char* data_ptr = static_cast<const char*>(data);

  while (total_sent < size)
  {
    try
    {
      int sent = sock_.snd(data_ptr, size - total_sent);

      if (sent == 0 && size > 0) {
        // Unexpected closing of file
        throw KeyenceException("Remote socket unexpectedly closed when attempting to send");
      }

      total_sent += sent;
      data_ptr += sent;
    }
    catch (const libsocket::socket_exception& ex)
    {
      throw KeyenceException(ex.mesg);
    }
  }

  return total_sent;
} 

int keyence::TcpClient::recv(void* data, size_t size)
{
  unsigned total_received = 0;
  char* data_ptr = static_cast<char*>(data);

  while (total_received < size)
  {
    try
    {
      int received = sock_.rcv(data_ptr, size - total_received);
      if (received == 0 && size > 0) {
        // Unexpected closing of file
        throw KeyenceException("Remote socket unexpectedly closed when attempting to recv");
      }

      total_received += received;
      data_ptr += received;
    }
    catch (const libsocket::socket_exception& ex)
    {
      throw KeyenceException(ex.mesg);
    }
  }
  return total_received;
}
