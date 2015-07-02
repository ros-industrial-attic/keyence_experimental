#ifndef KEYENCE_HIGH_SPEED_SINGLE_PROFILE_H
#define KEYENCE_HIGH_SPEED_SINGLE_PROFILE_H

#include "../keyence_message.h"
#include "../high_speed_defs.h"

namespace keyence
{
namespace command
{

class SingleProfile
{
public:
  // Forward declares
  struct Request;
  struct Response;

  struct Request
  {
    typedef Response response_type;
    
    const static uint32_t size = 16;
    const static uint8_t command_code = 0x42;

    // Required encode function
    void encodeInto(MutableBuffer buffer);
  };

  struct Response
  {
    typedef int32_t profile_pt_t;

    ProfileInformation profile_info;
    uint32_t trigger_count;
    uint32_t encoder_count;
    std::vector<profile_pt_t> profile_points;
  
    void decodeFrom(MutableBuffer buffer);
  };

};

} // namespace command
} // namespace keyence

#endif
