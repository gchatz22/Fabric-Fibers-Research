//  I2C Scanner from Arduino.cc
 
#include <Wire.h>
 
void setup()
{
  Wire.begin();
 
  Serial.begin(9600); //can be change to 115200
  finder();
}
 
void finder()
{
  byte error, address;
  int nDevices;
 
 
  nDevices = 0;
  for(address = 1; address <= 127; address++)
  {
    // The i2c_scanner uses the return value of
    // Wire.endTransmission to see if
    // a device acknowledged the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
 
  delay(5000);           // wait 5 seconds for next scan
}

void loop(){
}
