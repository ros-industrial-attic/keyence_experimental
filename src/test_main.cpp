#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/messages/change_program.h"

#include <cstdlib>

int main(int argc, char** argv)
{
  try
  {
    using keyence::command::ChangeProgram;
    using keyence::Client;

    if (argc != 4) return -1;

    keyence::TcpClient keyence (argv[1], argv[2]);

    ChangeProgram::Request req(std::atoi(argv[3]));

    Client::Response<ChangeProgram::Request> resp = keyence.sendRecieve(req);

    std::cout << resp.header << '\n';

  }
  catch (const libsocket::socket_exception& exc)
  {
    std::cerr << exc.mesg;
  }
  
return 0;
}