#ifndef KEYENCE_HIGH_SPEED_TCP_H
#define KEYENCE_HIGH_SPEED_TCP_H

#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/high_speed_defs.h"

#include <boost/function.hpp>

namespace keyence
{

class HighSpeedTcpClient
{
public:
  // Callback types
  typedef boost::function<void(int32_t*, uint32_t)> callback_type;

  // Constructors
  HighSpeedTcpClient(TcpClient& client, const std::string& port);

  ~HighSpeedTcpClient();

  const ProfileInformation& profileInfo() const;

  void start(callback_type cb);

  void stop();

 private:
  HighSpeedTcpClient operator=(const HighSpeedTcpClient&); // deleted
  HighSpeedTcpClient(const HighSpeedTcpClient&); // deleted


  TcpClient& client_;
  // Pimpl technique
  struct Impl;
  Impl* impl_;
};

}

#endif // KEYENCE_HIGH_SPEED_TCP_H

