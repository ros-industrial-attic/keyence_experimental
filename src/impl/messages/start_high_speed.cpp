#include "keyence/impl/messages/start_high_speed.h"

keyence::command::StartHighSpeed::Request::Request(uint32_t code)
  : start_code (code)
{
}

void keyence::command::StartHighSpeed::Request::encodeInto(MutableBuffer buffer)
{
  using keyence::insert;

  insert(buffer.data, static_cast<uint8_t>(0x47), 0);
  insert(buffer.data, static_cast<uint8_t>(0x0), 1);
  insert(buffer.data, static_cast<uint8_t>(0x0), 2);
  insert(buffer.data, static_cast<uint8_t>(0x0), 3);
  insert(buffer.data, static_cast<uint32_t>(start_code), 4);
}
