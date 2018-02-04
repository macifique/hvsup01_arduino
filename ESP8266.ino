#include <Wire.h>
#include "hvsup01.h"

static GMSup radcounter;
uint16_t vset = 400; //hvsup is set to 400V
uint16_t tset = 60; //hvsup triggers every 60 seconds
void serve();
void getter();

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  radcounter.begin(400000);
  getter();
  
  attachInterrupt(0, serve, FALLING);
  serve();
}

void loop() {
  
  //delay(2000);                  // waits for a second
  //getter();

}
void getter() {
  unsigned long serial_no;
  unsigned short hw_no, fw_no;
  
  radcounter.getinfo(serial_no, fw_no, hw_no);
  Serial.println("Rad Counter device info:");
  Serial.print("Firmware Version: ");
  Serial.println(fw_no, HEX);
  Serial.print("Hardware Version: ");
  Serial.println(hw_no, HEX);
  Serial.print("SerialNo: ");
  Serial.println(serial_no, HEX);
}

void serve() {
  int incomingByte = 0;
  unsigned long pulsecount;
  unsigned short voltage;
  unsigned short timeout;

  //delay(50);
  radcounter.getparams(pulsecount, voltage, timeout);
  if (voltage != vset) {
    Serial.print("Voltage mismatch. Read out voltage is:");
    Serial.println(voltage);
    Serial.print("Setting voltage to ");
    Serial.print(vset);
    Serial.println("V ...");
    
    radcounter.setparams(vset, tset);
  } else {
    Serial.print("Pulsecount: ");
    Serial.println(pulsecount);
  }
}
