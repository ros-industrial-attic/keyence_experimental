#ifndef KEYENCE_CHANGE_PROGRAM_H
#define KEYENCE_CHANGE_PROGRAM_H

#include "../keyence_message.h"
#include "../keyence_utils.h"

namespace keyence
{
namespace command
{

class ChangeProgram
{
public:
  // Forward declares
  struct Request;
  struct Response;

  struct Request 
  {
    typedef Response response_type;
    const static uint32_t size = 4;
    const static uint8_t command_code = 0x39;

    Request(uint8_t program_no) : program_no(program_no)
    {}

    void encodeInto(MutableBuffer buffer)
    {
      insert(buffer.data, program_no);
    }

    uint8_t program_no;
  };

  struct Response
  {
    void decodeFrom(MutableBuffer)
    {}
  };
};

} // namespace command
} // namespace keyence

#endif
