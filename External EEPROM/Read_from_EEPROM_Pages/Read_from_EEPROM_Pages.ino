//Include the Wire I2C Library
#include <Wire.h>

int EEPROM_ADR = 87;

void setup()
{

//Start the I2C Library
  Wire.setClock(100000);

//Start the serial port
  Serial.begin(19200);


  for (long x = 0 ; x < 16000 ; x++) //Read all 16,000 bytes from EERPOM
  {
    byte val = readEEPROM(x);
    Serial.write(val);
  }
}

void loop()
{
  //Nothing to do, just hang out.
}


byte readEEPROM(long eeaddress)
{
  Wire.beginTransmission(EEPROM_ADR);

  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_ADR, 1);

  byte rdata = 0xFF;
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}
