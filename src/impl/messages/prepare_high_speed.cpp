#include "keyence/impl/messages/prepare_high_speed.h"

keyence::command::PrepareHighSpeed::Request::Request(StartPosition position)
{
  switch (position)
  {
  case POSITION_PREVIOUS:
    send_position = 0; break;
  case POSITION_OLDEST:
    send_position = 1; break;
  case POSITION_NEXT:
    send_position = 2; break;
  }
}

void keyence::command::PrepareHighSpeed::Request::encodeInto(MutableBuffer buffer)
{
  using keyence::insert;

  insert(buffer.data, static_cast<uint8_t>(send_position), 0);
  insert(buffer.data, static_cast<uint8_t>(0x0), 1);
  insert(buffer.data, static_cast<uint8_t>(0x0), 2);
  insert(buffer.data, static_cast<uint8_t>(0x0), 3);
}

void keyence::command::PrepareHighSpeed::Response::decodeFrom(MutableBuffer buffer)
{
  using keyence::extract;

  extract<uint32_t>(buffer.data, start_code, 4);

  // extract profile info
  extract(buffer.data, profile_info.num_profiles, 16);
  extract(buffer.data, profile_info.data_unit, 18);
  extract(buffer.data, profile_info.x_start, 20);
  extract(buffer.data, profile_info.x_increment, 24);
}
