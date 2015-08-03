#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"
// For given message
#include "keyence/impl/keyence_high_speed_tcp.h"

#include <boost/bind.hpp>

void process(const int32_t* data, uint32_t size)
{
  std::cout << "cb: " << *data << " " << size << '\n';
}

struct Handler
{
  keyence::ProfileInformation info;
  void handle(const int32_t* data, uint32_t size)
  {
    std::cout << "struct cb" << *data << " " << size << "\n";
  }
};

int main(int argc, char** argv)
{
  using keyence::Client;

  if (argc != 3)
  {
    std::cerr << "Usage: ./keyence_change_program <host> <port>\n";
    return 1;
  }

  try
  {
    keyence::TcpClient client (argv[1], argv[2]);

    keyence::HighSpeedTcpClient high_speed (client, "24692");

    high_speed.start(process);

// An example of how to use member functions with the callback scheme:
//    Handler handler;
//    handler.info = high_speed.profileInfo();
//    high_speed.start( boost::bind(&Handler::handle, &handler, _1, _2) );

    ::sleep(5);
    std::cout << "Stopping.\n";
    high_speed.stop();

    ::sleep(2);
    std::cout << "Done\n";
  }
  catch (const keyence::KeyenceException& exc)
  {
    std::cerr << exc.what() << std::endl;
    return -1;
  }

  return 0;
}



