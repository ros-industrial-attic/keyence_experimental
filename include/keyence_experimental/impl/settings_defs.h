#ifndef SETTINGS_DEFS_H
#define SETTINGS_DEFS_H

#include <stdint.h>

namespace keyence
{
namespace setting
{

/**
 * The keyence controller keeps several copies of its settings around.
 * Updates appear to be transactional: The user writes into the
 * 'write_area' and then must 'reflect' those changes to the running_area,
 * the 'save area' or both.
 */
const static uint8_t write_area = 0;
const static uint8_t running_area = 1;
const static uint8_t save_area = 2;

/**
 * The keyence controller supports 16 different programs, 0 to 15
 */
const static uint8_t min_program_index = 0;
const static uint8_t max_program_index = 15;

/**
 * Program data is referenced using type = 0x10 for program 0,
 * type = 0x11 for program 1, and so on until 0x1F. See the
 * individual types for category and item information as well
 * as data types particular to each setting.
 */
namespace program
{

/**
 * @brief Returns the 'type' value appropriate for the given logical program
 * number. This field goes into the 'type' field of the get/set settings
 * commands.
 * @param logical_program_no The index of the user program you wish to get the
 * associated type for. Valid inputs are 0 - 15.
 * @return Integer that can be used in keyence::command::GetSetting
 */
inline uint8_t programType(uint8_t logical_program_no)
{
  return 0x10 + logical_program_no;
}

struct TriggerMode
{
  static const uint8_t category = 0x00;
  static const uint8_t item = 0x01;

  static const uint8_t continuous_trigger = 0;
  static const uint8_t external_trigger = 1;
  static const uint8_t encoder_trigger = 2;
};

struct SamplingPeriod
{
  static const uint8_t category = 0x00;
  static const uint8_t item = 0x02;

  static const uint8_t freq_10hz = 0;
  static const uint8_t freq_20hz = 1;
  static const uint8_t freq_50hz = 2;
  static const uint8_t freq_100hz = 3;
  static const uint8_t freq_200hz = 4;
  static const uint8_t freq_500hz = 5;
  static const uint8_t freq_1000hz = 6;
  static const uint8_t freq_2000hz = 7;
  static const uint8_t freq_4000hz = 8;
  static const uint8_t freq_4130hz = 9;
  static const uint8_t freq_8000hz = 10;
  static const uint8_t freq_16000hz = 11;
  static const uint8_t freq_32000hz = 12;
  static const uint8_t freq_64000hz = 13;
};

} // ns program

}
}

#endif // SETTINGS_DEFS_H
