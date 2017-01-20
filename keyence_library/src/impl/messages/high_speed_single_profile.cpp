#include "keyence/impl/messages/high_speed_single_profile.h"
#include "keyence/impl/keyence_utils.h"
#include "keyence/impl/ljv7_rawdata.h"

void keyence::command::SingleProfile::Request::encodeInto(MutableBuffer buffer)
{
  using keyence::insert;

  insert(buffer.data, static_cast<uint32_t>(0x00000000), 0);
  insert(buffer.data, static_cast<uint32_t>(0x00000000), 4);
  insert(buffer.data, static_cast<uint32_t>(0x1), 8);
  insert(buffer.data, static_cast<uint32_t>(0x00000101), 12);
}

void keyence::command::SingleProfile::Response::decodeFrom(MutableBuffer buffer)
{
  using keyence::extract;

  extract(buffer.data, profile_info.num_profiles, 24);
  extract(buffer.data, profile_info.data_unit, 26);
  extract(buffer.data, profile_info.x_start, 28);
  extract(buffer.data, profile_info.x_increment, 32);
  extract(buffer.data, trigger_count, 40);
  extract(buffer.data, encoder_count, 44);

  // Prepare room for the profiles
  profile_points.resize(profile_info.num_profiles);

  unsigned char* src_data = static_cast<unsigned char *>(buffer.data)+60;

  ljv7_unpack_profile_data(src_data, // buffer of data from keyence
                           profile_info.num_profiles * 5 / 2, // size of keyence data (20 bits per profile)
                           profile_info.num_profiles, // number of profile points
                           profile_points.data(), // output buffer
                           profile_points.size() * sizeof(profile_pt_t)); // output buffer size (bytes)
}
