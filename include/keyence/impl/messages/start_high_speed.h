#ifndef START_HIGH_SPEED_H
#define START_HIGH_SPEED_H

#include "../keyence_message.h"
#include "../high_speed_defs.h"

namespace keyence
{
namespace command
{

class StartHighSpeed
{
public:
  // Forward declares
  struct Request;
  struct Response;

  struct Request
  {
    // Required definitions
    typedef Response response_type;
    const static uint32_t size = 8;
    const static uint8_t command_code = 0xA0;

    Request(uint32_t code);

    // Request Data
    uint32_t start_code;

    // Required encode function
    void encodeInto(MutableBuffer buffer);
  };

  struct Response
  {
    void decodeFrom(MutableBuffer) {}
  };

};

} // namespace command
} // namespace keyence



#endif // START_HIGH_SPEED_H

