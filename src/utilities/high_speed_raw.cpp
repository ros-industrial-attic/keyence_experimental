#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"
// For given message
#include "keyence/impl/messages/prepare_high_speed.h"
#include "keyence/impl/messages/start_high_speed.h"
#include "keyence/impl/messages/stop_high_speed.h"
#include <unistd.h>

int main(int argc, char** argv)
{
  using keyence::command::PrepareHighSpeed;
  using keyence::command::StartHighSpeed;
  using keyence::command::StopHighSpeed;
  using keyence::Client;

  if (argc != 3)
  {
    std::cerr << "Usage: ./keyence_change_program <host> <port>\n";
    return 1;
  }

  try
  {
    // regular keyence connection
    keyence::TcpClient keyence (argv[1], argv[2]);
    // open up parallel high-speed connection
    libsocket::inet_stream hs_sock (argv[1], argv[3], LIBSOCKET_IPv4);

    // Prepare for high speed
    PrepareHighSpeed::Request prepare (PrepareHighSpeed::Request::POSITION_NEXT);
    Client::Response<PrepareHighSpeed::Request> prepare_resp = keyence.sendReceive(prepare);

    std::cout << prepare_resp.header << '\n';

    // Start
    StartHighSpeed::Request start (prepare_resp.body.start_code);
    Client::Response<StartHighSpeed::Request> start_resp = keyence.sendReceive(start);

    std::cout << start_resp.header << '\n';

    // Read from the hs socket...
    // Check the body of the prepare-response for details about the profile size
    ::sleep(10);

  }
  catch (const keyence::KeyenceException& exc)
  {
    std::cerr << exc.what();
    return -1;
  }

  return 0;
}


