#include "keyence/impl/messages/get_setting.h"
#include "keyence/impl/keyence_utils.h"

keyence::command::GetSetting::Request::Request(uint8_t level, uint8_t type,
                                                  uint8_t category, uint8_t item,
                                                  uint8_t target1, uint8_t target2,
                                                  uint8_t target3, uint8_t target4)
  : level(level), type(type)
  , category(category), item(item)
  , target1(target1), target2(target2)
  , target3(target3), target4(target4)
{
  using keyence::insert;
}

void keyence::command::GetSetting::Request::encodeInto(keyence::MutableBuffer buffer)
{
  insert(buffer.data, static_cast<uint32_t>(0x00000000), 0);
  insert(buffer.data, level, 4);
  insert(buffer.data, static_cast<uint8_t>(0x0), 5);
  insert(buffer.data, static_cast<uint8_t>(0x0), 6);
  insert(buffer.data, static_cast<uint8_t>(0x0), 7);
  insert(buffer.data, type, 8);
  insert(buffer.data, category, 9);
  insert(buffer.data, item, 10);
  insert(buffer.data, static_cast<uint8_t>(0x0), 11);
  insert(buffer.data, target1, 12);
  insert(buffer.data, target2, 13);
  insert(buffer.data, target3, 14);
  insert(buffer.data, target4, 15);
}

void keyence::command::GetSetting::Response::decodeFrom(keyence::MutableBuffer resp)
{
  data.resize(resp.size);
  memcpy(data.data(), resp.data, resp.size);
}
