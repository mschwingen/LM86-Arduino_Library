/******************************************************************************
LM86.cpp
Arduino Library for the LM86 local/remote temperature sensor

(C) 2024 Michael Schwingen <michael@schwingen.org>

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
******************************************************************************/

#include "LM86.h"

//The 7-bit I2C ADDRESS of the LM86
#define LM86_ADDR ((uint8_t) 0x4C)

// register addresses (read)
enum class LM86::RegAddrRd: uint8_t {
  LT = 0x00,
  RTHB = 0x01,
  SR = 0x02,
  C = 0x03,
  CR = 0x04,
  RTLB = 0x10,
  RTOHB = 0x11,
  RTOLB = 0x12,
  RDTF = 0xBF,
  RMID = 0xFE,
  RDR = 0xFF
};

// register addresses (write)
enum class LM86::RegAddrWr: uint8_t {
  LT = 0x00,
  SR = 0x02,
  C = 0x09,
  CR = 0x0A,
  ONESHOT = 0x0F,
  RTOHB = 0x11,
  RTOLB = 0x12,
  RDTF = 0xBF
};


// init LM86 and setup conversion rate.
bool LM86::begin(TwoWire &wirePort, ConvRate rate) {
  bool rc;
  _i2cPort = &wirePort;
  uint8_t id1, id2;

  rc = readRegister(RegAddrRd::RMID, id1);
  rc &= readRegister(RegAddrRd::RDR, id2);
  //Serial.printf("LM86: ID %02x, %02x", id1, id2);

  if (rate == LM86::ConvRate::CR_ONESHOT) {
    rc &= writeRegister(RegAddrWr::CR, (uint8_t) LM86::ConvRate::CR_62mHz);
    rc &= writeRegister(RegAddrWr::C, 0x40); // enter shutdown
  } else {
    rc &= writeRegister(RegAddrWr::CR, (uint8_t) rate);
    rc &= writeRegister(RegAddrWr::C, 0x00);
  }

  uint8_t x;
  rc &= readRegister(RegAddrRd::CR, x);
  //Serial.printf("\nLM86: CR=%02x", x);
  rc &= readRegister(RegAddrRd::C, x);
  //Serial.printf("\nLM86: C=%02x", x);

  if (id1 != 0x01 || id2 != 0x11)
    rc = false;

  init_ok = rc;
  return rc;
}


// trigger a single conversion and wait until done.
bool LM86::oneshot(void)
{
  bool rc;
  uint8_t sr;
  int timeout;

  if (!init_ok)
    return false;

  rc = writeRegister(RegAddrWr::ONESHOT, 0x00);
  if (!rc)
    return rc;
  delay(32); // conversion takes 32.5ms
  timeout = 50;
  do
  {
    rc = readRegister(RegAddrRd::SR, sr);
    if (!rc)
      return rc;
    if (--timeout == 0)
      return false;
    delay(1);
  } while (sr & 0x80); // wait until not busy
  rc &= writeRegister(RegAddrWr::C, 0x40); // enter shutdown
  return rc;
}


// read the local (LM86 chip) temperature
float LM86::getLocalTemperature(void)
{
  bool rc = false;
  uint8_t temp;

  if (init_ok)
    rc = readRegister(RegAddrRd::LT, temp);
  if (!rc)
    return -1000.0;
  return (float) temp;
}


// read the remote (external diode) temperature
float LM86::getRemoteTemperature(void)
{
  bool rc = false;
  uint8_t temp_h, temp_l;
  uint16_t temp;

  if (init_ok) {
    rc = readRegister(RegAddrRd::RTHB, temp_h);
    rc &= readRegister(RegAddrRd::RTLB, temp_l);
  }
  if (!rc)
    return -1000.0;
  temp = ((uint16_t) temp_h << 8) | temp_l;
  return (float) temp / 256.0;
}


// read one register
bool LM86::readRegister(RegAddrRd addr, uint8_t &result) {
  _i2cPort->beginTransmission(LM86_ADDR);
  _i2cPort->write((uint8_t) addr);
  if (_i2cPort->endTransmission() != 0)
    return false; //Error

  if (_i2cPort->requestFrom(LM86_ADDR, (uint8_t) 1) != 1)
    return false; //Error
  if (_i2cPort->available()) {
    result = _i2cPort->read();
    return true;
  }
  else {
    result = 0x00;
    return false; //Error
  }
}


// write one register
bool LM86::writeRegister(RegAddrWr addr, uint8_t val) {
  _i2cPort->beginTransmission(LM86_ADDR);
  _i2cPort->write((uint8_t) addr);
  _i2cPort->write(val);
  return _i2cPort->endTransmission() == 0;
}

