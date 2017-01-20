#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"
// For given message
#include "keyence/impl/messages/get_setting.h"

#include <cstdlib> // for atoi

uint8_t safeCast(int x)
{
  if (x < 0 || x > 255)
  {
    throw std::invalid_argument("Integer " + std::to_string(x) +
                                " can't be converted to uint8_t");
  }

  return static_cast<uint8_t>(x);
}

int main(int argc, char** argv)
{
  using keyence::command::GetSetting;
  using keyence::Client;

  if (argc != 11)
  {
    std::cerr << "Usage: ./keyence_change_program <host> <port> "
                 "<level> <type> <category> <item> <target1>"
                 " <target2> <target3> <target4>\n";
    return 1;
  }

  try
  {
    keyence::TcpClient keyence (argv[1], argv[2]);

    uint8_t level = safeCast(std::stoi(argv[3]));
    uint8_t type = safeCast(std::stoi(argv[4]));
    uint8_t category = safeCast(std::stoi(argv[5]));
    uint8_t item = safeCast(std::stoi(argv[6]));
    uint8_t target[4];
    target[0] = safeCast(std::stoi(argv[7]));
    target[1] = safeCast(std::stoi(argv[8]));
    target[2] = safeCast(std::stoi(argv[9]));
    target[3] = safeCast(std::stoi(argv[10]));

    GetSetting::Request req (level, type, category, item, target[0],
        target[1], target[2], target[4]);

    auto resp = keyence.sendReceive(req);

    std::cout << resp.header << '\n';

    if (resp.good())
    {
      for (auto&& b : resp.body.data)
      {
        std::cout << "Data: " << int(b) << "\n";
      }
    }
  }
  catch (const keyence::KeyenceException& exc)
  {
    std::cerr << exc.what();
    return -1;
  }

  return 0;
}
