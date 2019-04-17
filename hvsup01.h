// Header file for using HVSUP01 with Arduino
//
// Written by Karoly Molnar
// Copyright (c) 2018 Macifique
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

#include <Wire.h>
#ifndef _GMSUP_H_
#define _GMSUP_H_
class GMSup {

  public:
    void begin(int clockspeed = 400000, int sda = 4, int scl = 5) {
      Wire.begin(sda,scl);
      Wire.setClock(clockspeed);
      Wire.setClockStretchLimit(40000);
    }

    void getparams(uint32_t &pulsecount, uint16_t &voltage, uint16_t &timeout) {
      uint8_t buffer[7];
      readdevice(0, 7, buffer);
      pulsecount = (uint32_t)buffer[0];
      pulsecount |= ((uint32_t)buffer[1]) << 8;
      pulsecount |= ((uint32_t)buffer[2]) << 16;
      timeout = (uint16_t)buffer[3] | (((uint16_t)buffer[4]) << 8);
      voltage = (uint16_t)buffer[5] | (((uint16_t)buffer[6]) << 8);
    }

    void getinfo(uint32_t &serialno, uint16_t &firmware_ver, uint16_t &hw_ver) {
      uint8_t buffer[8];
      readdevice(7, 8, buffer);
      hw_ver = (uint16_t)buffer[0] | (((uint16_t)buffer[1]) << 8);
      firmware_ver = (uint16_t)buffer[2] | (((uint16_t)buffer[3]) << 8);
      serialno = (uint32_t)buffer[4] |
                 (((uint32_t)buffer[5]) << 8) |
                 (((uint32_t)buffer[6]) << 16) |
                 (((uint32_t)buffer[7]) << 24);
    }

    /* Get pulse cont from GM supply
        input parameters: none
        output:
        uint32_t  - number of pulses since the last read or last timeout
    */
    uint32_t getpulsecount(void) {
      uint8_t buffer[3];
      uint32_t pulsecount;
      readdevice(0, 3, buffer);
      pulsecount = (uint32_t)buffer[0];
      pulsecount |= ((uint32_t)buffer[1]) << 8;
      pulsecount |= ((uint32_t)buffer[2]) << 16;
      return pulsecount;
    }



    /* Initialize GM supply
        input parameters:
        uint16_t voltage - operating voltage of the GM tube. range: 400..600
                               - value lower than 400 will turn off the HV stage
        uint32_t timeout - periodic triger from GMSUP-1 via TRIG pin. range 0..65535sec
                              - value 0 will turn off trigger
    */
    void setparams(uint16_t voltage, uint16_t timeout = 0) {
      uint8_t buf[4];
      buf[0] = timeout;
      buf[1] = timeout >> 8;
      buf[2] = voltage;
      buf[3] = voltage >> 8;
      writedevice(3, 4, buf);
    }

  protected:
    void readdevice(uint8_t address, uint8_t len, uint8_t *data) {
      Wire.beginTransmission(deviceaddress);
      Wire.write(address);
      Wire.endTransmission();
      Wire.requestFrom(deviceaddress, len, true);
      while (Wire.available()) {
        *data++ = Wire.read();
      }
    }

    void writedevice(uint8_t address, uint8_t len, uint8_t *data) {
      Wire.beginTransmission(deviceaddress);
      Wire.write(address);
      Wire.write(data, len);
      Wire.endTransmission(true);
    }
    const uint8_t deviceaddress = 0x6e;
};
#endif //_GMSUP_H_
