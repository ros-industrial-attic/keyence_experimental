#ifndef KEYENCE_HIGH_SPEED_DEFS_H
#define KEYENCE_HIGH_SPEED_DEFS_H

#include <stdint.h>

#define KEYENCE_FUNDAMENTAL_LENGTH_UNIT (0.01 * 1.0e-6)

// See LJ-V7000 Communications Library Reference Manual S-9.2.9.5
/*
 * The above section defines 0x80000000 - 0x80000003 as invalid values
 * but for high-speed mode, these appear to be fit into the available
 * twenty bits. So 0x80000 - 0x80003 are the new invalid ranges.
 */
// "This value is output when the peak could not be detected."
#define KEYENCE_INVALID_DATA_VALUE -524288
// "This value is output when the data is invalid due to
//  a setting such as a mask having been set".
#define KEYENCE_INVALID_DATA_VALUE2 -524287
// "This value is output when the data is located in a dead zone.
//  This value is only output when dead zone processing is enabled."
#define KEYENCE_DEAD_ZONE_DATA_VALUE -524286
// "This value is output when there is an insufficient amount of profiles for averaging."
#define KEYENCE_JUDGEMENT_WAIT_DATA_VALUE -524285

namespace keyence
{

inline bool isPointValid(int32_t pt)
{
  return pt > KEYENCE_JUDGEMENT_WAIT_DATA_VALUE;
}

/**
 * @brief The ProfileInformation struct
 *
 * The fundamental keyence base-unit of length is 0.01 micrometers.
 * Therefore a 50 in the 'data_unit' field means that the value sent
 * over for a given depth measurement is in units of 50 * 0.01 = 0.5 um.
 */
struct ProfileInformation
{
  uint16_t num_profiles;
  uint16_t data_unit; // equivalent to micrometers
  int32_t x_start;
  int32_t x_increment;
};

inline double unitsToMeters(int32_t unit)
{
  return unit * KEYENCE_FUNDAMENTAL_LENGTH_UNIT;
}

}

#endif
