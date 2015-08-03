#ifndef STOP_HIGH_SPEED_H
#define STOP_HIGH_SPEED_H

namespace keyence
{
namespace command
{

class StopHighSpeed
{
public:
  // Forward declares
  struct Request;
  struct Response;

  struct Request
  {
    // Required definitions
    typedef Response response_type;
    const static uint32_t size = 0;
    const static uint8_t command_code = 0x48;

    // Required encode function
    void encodeInto(MutableBuffer) {}
  };

  struct Response
  {
    void decodeFrom(MutableBuffer) {}
  };

};

} // namespace command
} // namespace keyence

#endif // STOP_HIGH_SPEED_H

