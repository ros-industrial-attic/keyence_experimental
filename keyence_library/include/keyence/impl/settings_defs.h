#ifndef SETTINGS_DEFS_H
#define SETTINGS_DEFS_H

#include <stdint.h>

namespace keyence
{
namespace setting
{

/**
 * Program data is referenced using type = 0x10 for program 0,
 * type = 0x11 for program 1, and so on until 0x1F. See the
 * individual types for category and item information as well
 * as data types particular to each setting.
 */
namespace program
{

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
