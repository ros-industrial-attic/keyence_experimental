#ifndef PREPARE_HIGH_SPEED_H
#define PREPARE_HIGH_SPEED_H

#include "../keyence_message.h"
#include "../high_speed_defs.h"

namespace keyence
{
namespace command
{

class PrepareHighSpeed
{
public:
  // Forward declares
  struct Request;
  struct Response;

  struct Request
  {
    // Required definitions
    typedef Response response_type;
    const static uint32_t size = 4;
    const static uint8_t command_code = 0x47;
    // Constructor
    enum StartPosition { POSITION_PREVIOUS, POSITION_OLDEST, POSITION_NEXT };

    Request(StartPosition position);

    // Request Data
    uint8_t send_position;

    // Required encode function
    void encodeInto(MutableBuffer buffer);
  };

  struct Response
  {
    ProfileInformation profile_info;
    uint32_t start_code;

    void decodeFrom(MutableBuffer buffer);
  };

};

} // namespace command
} // namespace keyence


#endif // PREPARE_HIGH_SPEED_H

