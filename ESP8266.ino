#include <Wire.h>
#include "hvsup01.h"

static GMSup radcounter;
uint16_t vset = 400; //hvsup is set to 400V
uint16_t tset = 60; //hvsup triggers every 60 seconds
void serve();
void getter();

void setup() {
  
  unsigned long serial_no;
  unsigned short hw_no, fw_no;
  
  Serial.begin(9600);
  Serial.println("Starting...");
  
  radcounter.begin(400000); // Initialize hvsup01 with 400kHz clock
  
  radcounter.getinfo(serial_no, fw_no, hw_no);  // Get factory data
  
  Serial.println("Rad Counter device info:");
  Serial.print("Firmware Version: ");
  Serial.println(fw_no, HEX);
  Serial.print("Hardware Version: ");
  Serial.println(hw_no, HEX);
  Serial.print("SerialNo: ");
  Serial.println(serial_no, HEX);
  
  attachInterrupt(0, serve, FALLING); // Atach interrupt of TRIG pin to 'serve'
  serve();  // ...and call serve first directly, to perform the initial setup
}

void loop() {
  // We use periodic interrupt, so nothing is here
}

void serve() {
  unsigned long pulsecount;
  unsigned short voltage;
  unsigned short timeout;

  radcounter.getparams(pulsecount, voltage, timeout); //get measurement data and settings
  if (voltage != vset) {  // check if setting is correct. This code runs normally once at startup
    Serial.print("Voltage mismatch. Read out voltage is:");
    Serial.println(voltage);
    Serial.print("Setting voltage to ");
    Serial.print(vset);
    Serial.println("V ...");
    
    radcounter.setparams(vset, tset); // Set the tube voltage and timeout
  } else {
    Serial.print("Pulsecount: ");
    Serial.println(pulsecount);
  }
}
