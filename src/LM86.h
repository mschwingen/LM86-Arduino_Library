#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include <time.h>

class LM86
{
public:
  LM86(void) {  }

  enum class ConvRate { // matches bit values in Conversion Rate (CR) register
    CR_62mHz = 0,
    CR_125mHz = 1,
    CR_250mHz = 2,
    CR_500mHz = 3,
    CR_1Hz = 4,
    CR_2Hz = 5,
    CR_4Hz = 6,
    CR_8Hz = 7,
    CR_16Hz = 8,
    CR_32Hz = 9,
    CR_ONESHOT = 10,
  };
  bool begin(TwoWire &wirePort = Wire, ConvRate rate = ConvRate::CR_1Hz);
  bool oneshot(void);
  float getLocalTemperature(void);
  float getRemoteTemperature(void);


private:
  enum class RegAddrRd: uint8_t; // device register addresses
  enum class RegAddrWr: uint8_t; // device register addresses
  TwoWire *_i2cPort;
  bool init_ok = false;

  bool readRegister(RegAddrRd addr, uint8_t &result);
  bool writeRegister(RegAddrWr addr, uint8_t val);
};
