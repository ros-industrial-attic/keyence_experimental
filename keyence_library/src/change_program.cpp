#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"
// For given message
#include "keyence/impl/messages/change_program.h"

#include <cstdlib> // for atoi

int main(int argc, char** argv)
{
  using keyence::command::ChangeProgram;
  using keyence::Client;

  if (argc != 4)
  {
    std::cerr << "Usage: ./keyence_change_program <host> <port> <program_no>\n";
    return 1;
  }

  try
  {
    keyence::TcpClient keyence (argv[1], argv[2]);

    ChangeProgram::Request req (std::atoi(argv[3]));

    Client::Response<ChangeProgram::Request> resp = keyence.sendReceive(req);

    std::cout << resp.header << '\n';
  }
  catch (const keyence::KeyenceException& exc)
  {
    std::cerr << exc.what();
    return -1;
  }
  
  return 0;
}
