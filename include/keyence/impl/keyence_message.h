#ifndef KEYENCE_MESSAGE_H
#define KEYENCE_MESSAGE_H

#include "keyence_buffer.h"
#include "keyence_utils.h"
#include <stdint.h>

#include <iostream>

namespace keyence
{

class Message
{
public:
  static const uint32_t prefix_size = 4;
  static const uint32_t request_header_size = 12 + 4;  //header plus first 4 bytes of body since it is constant
  static const uint32_t response_header_size = 12 + 12;  //header plus first 12 bytes of body since it is constant

  struct RequestHeader
  {
    // Actual header info
    uint32_t body_length;
    // Fixed Body Segment
    uint8_t command_code;
  };

  struct ResponseHeader
  {
    // Actual header info
    uint8_t header_return_code;
    uint32_t body_length;
    // Fixed Body Segment
    uint8_t command_code;
    uint8_t return_code;
    uint8_t controller_status;
    uint8_t active_program_no;
  };
};

inline void encodeRequestHeader(uint8_t code, uint32_t size, MutableBuffer buffer)
{
  insert(buffer.data, static_cast<uint32_t>(Message::request_header_size + size), 0);
  insert(buffer.data, static_cast<uint32_t>(0x00F00001), 4); // fixed
  insert(buffer.data, static_cast<uint32_t>(0x00000000), 8); // fixed
  insert(buffer.data, size + 4, 12); // body size
  insert(buffer.data, code, 16); // command code
  insert(buffer.data, static_cast<uint8_t>(0), 17); // fixed
  insert(buffer.data, static_cast<uint8_t>(0), 18); // fixed
  insert(buffer.data, static_cast<uint8_t>(0), 19); // fixed
}

inline void decodeResponseHeader(MutableBuffer buffer, Message::ResponseHeader& header)
{
  extract(buffer.data, header.header_return_code, 4);
  extract(buffer.data, header.body_length, 8);
  extract(buffer.data, header.command_code, 12);
  extract(buffer.data, header.return_code, 13);
  extract(buffer.data, header.controller_status, 14);
  extract(buffer.data, header.active_program_no, 20);
}

inline std::ostream& operator<<(std::ostream& os, const Message::ResponseHeader& hdr)
{
  os << "Header return code: " << static_cast<int>(hdr.header_return_code) << '\n';
  os << "Body length: " << static_cast<int>(hdr.body_length) << '\n';
  os << "Command code: " << static_cast<int>(hdr.command_code) << '\n';
  os << "Command return code: " << static_cast<int>(hdr.return_code) << '\n';
  os << "Controller status: " << static_cast<int>(hdr.controller_status) << '\n';
  os << "Active program no: " << static_cast<int>(hdr.active_program_no) << '\n'; 
  return os;
}

}

#endif
