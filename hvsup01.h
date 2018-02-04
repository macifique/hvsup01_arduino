#include <Wire.h>
#ifndef _GMSUP_H_
#define _GMSUP_H_
class GMSup {

  public:
    void begin(int clockspeed = 400000) {
      Wire.begin();
      Wire.setClock(clockspeed);
      Wire.setClockStretchLimit(40000);
    }

    void getparams(unsigned long &pulsecount, unsigned short &voltage, unsigned short &timeout) {
      byte buffer[7];
      readdevice(0, 7, buffer);
      pulsecount = (unsigned long)buffer[0];
      pulsecount |= ((unsigned long)buffer[1]) << 8;
      pulsecount |= ((unsigned long)buffer[2]) << 16;
      timeout = (unsigned short)buffer[3] | (((unsigned short)buffer[4]) << 8);
      voltage = (unsigned short)buffer[5] | (((unsigned short)buffer[6]) << 8);
    }

    void getinfo(unsigned long &serialno, unsigned short &firmware_ver, unsigned short &hw_ver) {
      byte buffer[8];
      readdevice(7, 8, buffer);
      hw_ver = (unsigned short)buffer[0] | (((unsigned short)buffer[1]) << 8);
      firmware_ver = (unsigned short)buffer[2] | (((unsigned short)buffer[3]) << 8);
      serialno = (unsigned long)buffer[4] |
                 (((unsigned long)buffer[5]) << 8) |
                 (((unsigned long)buffer[6]) << 16) |
                 (((unsigned long)buffer[7]) << 24);
    }

    /* Get pulse cont from GM supply
        input parameters: none
        output:
        unsigned long  - number of pulses since the last read or last timeout
    */
    unsigned long getpulsecount(void) {
      byte buffer[3];
      unsigned long pulsecount;
      readdevice(0, 3, buffer);
      pulsecount = (unsigned long)buffer[0];
      pulsecount |= ((unsigned long)buffer[1]) << 8;
      pulsecount |= ((unsigned long)buffer[2]) << 16;
      return pulsecount;
    }



    /* Initialize GM supply
        input parameters:
        unsigned short voltage - operating voltage of the GM tube. range: 400..600
                               - value lower than 400 will turn off the HV stage
        unsigned long timeout - periodic triger from GMSUP-1 via TRIG pin. range 0..65535sec
                              - value 0 will turn off trigger
    */
    void setparams(unsigned short voltage, unsigned short timeout = 0) {
      byte buf[4];
      buf[0] = timeout;
      buf[1] = timeout >> 8;
      buf[2] = voltage;
      buf[3] = voltage >> 8;
      writedevice(3, 4, buf);
    }

  protected:
    void readdevice(byte address, byte len, byte *data) {
      Wire.beginTransmission(deviceaddress);
      Wire.write(address);
      Wire.endTransmission();
      Wire.requestFrom(deviceaddress, len, true);
      while (Wire.available()) {
        *data++ = Wire.read();
      }
    }

    void writedevice(byte address, byte len, byte *data) {
      Wire.beginTransmission(deviceaddress);
      Wire.write(address);
      Wire.write(data, len);
      Wire.endTransmission(true);
    }
    const byte deviceaddress = 0x6e;
};
#endif //_GMSUP_H_
