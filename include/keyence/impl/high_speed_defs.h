#ifndef KEYENCE_HIGH_SPEED_DEFS_H
#define KEYENCE_HIGH_SPEED_DEFS_H

#include <stdint.h>

namespace keyence
{

struct ProfileInformation
{
  uint16_t num_profiles;
  uint16_t data_unit; // equivalent to micrometers
  int32_t x_start;
  int32_t x_increment;
};

}

#endif
