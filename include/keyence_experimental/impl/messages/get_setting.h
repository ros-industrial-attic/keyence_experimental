#ifndef KEYENCE_GET_SETTING_H
#define KEYENCE_GET_SETTING_H

#include "../keyence_message.h"

namespace keyence
{
namespace command
{

class GetSetting
{
public:
  // Forward declares
  struct Request;
  struct Response;

  struct Request
  {
    typedef Response response_type;
    const static uint32_t size = 16;
    const static uint8_t command_code = 0x31;

    Request   (uint8_t level, uint8_t type, uint8_t category,
               uint8_t item, uint8_t target1, uint8_t target2,
               uint8_t target3, uint8_t target4);

    void encodeInto(MutableBuffer buffer);

    uint8_t level;
    uint8_t type;
    uint8_t category;
    uint8_t item;
    uint8_t target1, target2, target3, target4;

  };

  struct Response
  {
    void decodeFrom(MutableBuffer resp);
    std::vector<uint8_t> data;
  };
};

} // namespace command
} // namespace keyence


#endif // KEYENCE_GET_SETTING_H
