//Include the Wire I2C Library
#include <Wire.h>

#define EEPROM_ADR 0x54

#define MAX_I2C_WRITE 16 

byte tempStore[MAX_I2C_WRITE];

void setup()
{

//Start the I2C Library
  Wire.begin();
  Wire.setClock(100000);

//Start the serial port
  Serial.begin(19200);


  long currentSpot = 0;
  long timerReset = 0;
  byte counter = 0;

  
  while (1)
  {
    while (Serial.available())
    {
      tempStore[counter++] = Serial.read(); //Read this byte into the array

      if (counter == MAX_I2C_WRITE)
      {
      //Once we've collected a page worth, go ahead and do 
      //a page write operation
        writeEEPROMPage(currentSpot);
        counter = 0; //Reset
        currentSpot += MAX_I2C_WRITE;
      }

      timerReset = millis();
    }

    if (millis() - timerReset > 2000)
    {
      Serial.println(currentSpot);
      timerReset = millis();
    }
  }

}

void loop()
{
    // Don't do anything here
}


void writeEEPROMPage(long eeAddress)
{

  Wire.beginTransmission(EEPROM_ADR);

  Wire.write((int)(eeAddress >> 8)); // MSB
  Wire.write((int)(eeAddress & 0xFF)); // LSB

  //Write bytes to EEPROM
  for (byte x = 0 ; x < MAX_I2C_WRITE ; x++)
    Wire.write(tempStore[x]); //Write the data

  Wire.endTransmission(); //Send stop condition
}
