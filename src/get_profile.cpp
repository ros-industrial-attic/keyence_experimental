#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"
// For given message
#include "keyence/impl/messages/high_speed_single_profile.h"

int main(int argc, char** argv)
{
  try
  {
    using keyence::command::SingleProfile;
    using keyence::Client;

    if (argc != 4)
    {
      std::cerr << "Usage: ./keyence_change_program <host> <port> <program_no>\n";
    }

    keyence::TcpClient keyence (argv[1], argv[2]);

    SingleProfile::Request req;

    Client::Response<SingleProfile::Request> resp = keyence.sendReceive(req);

    std::cout << resp.header << '\n';
  }
  catch (const keyence::KeyenceException& exc)
  {
    std::cerr << exc.what();
  }

return 0;
}

