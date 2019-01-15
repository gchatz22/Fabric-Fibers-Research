#include <Wire.h> //Required library to communicate with EEPROM

#define MAX_ADR 16000 //Depends on storage of each EEPROM. For 128K it is 16000, for 256K is 32000
#define MAX_I2C_WRITE 16 //This is for Page Write. We write in the EEPROM every 16 bytes we receive

int addresses[8]={0,0,0,0,0,0,0,0}; //array containing all the available addresses
int EEPROM_ADR; //the EEPROM address in which we write each time

byte tempStore[MAX_I2C_WRITE]; //an array in which we temporarily store the input bytes (one at each index) to later store them in the EEPROM

void setup()
{

  Serial.begin(19200);
  Wire.begin();
  Wire.setClock(100000); //clock frequency for 10kHz resistor

  devices_finder(); //function that finds all the available devices on the board
  EEPROM_ADR=addresses[0];

  long currentSpot = 0;
  long timerReset = 0;
  byte counter = 0;
  long write_to;
  bool guard=true;
  int devices=0;


  //WRITING OPERATION
  
  
  while (guard==true || (millis() - timerReset < 2000))
  {
    while (Serial.available()) //while the serial monitor receives data
    {

      tempStore[counter++] = Serial.read();//Read this byte into the array
  
      if (counter == MAX_I2C_WRITE)
      {
      //Once we've collected a page worth, go ahead and do 
      //a page write operation
        write_page(currentSpot,MAX_I2C_WRITE);
        counter = 0; //Reset
        currentSpot += MAX_I2C_WRITE; //update current spot
      }
  
      guard=false; //set guard to false so that we write only once each time
      timerReset = millis();
  
  
      if (currentSpot==MAX_ADR) { //if we reach the maximum storage of a device
  
        if (devices==7){ //if we have used the maximum number (8) of devices available on the board

          Serial.println("Max storage of all memories reached");
          break;
          
        }

        else{ //switch to the next device in the array and reset the current spot

          ++devices;
          EEPROM_ADR=addresses[devices];
          currentSpot=0;
          Serial.print("Switched to memory ");
          Serial.println(EEPROM_ADR);
          
        }
           
     }

   }
    
    if (millis() - timerReset > 2000) //wait for data by outputting the current spot each time
    {
      
      Serial.println(currentSpot);
      timerReset = millis();
    }
  }

  if (counter < MAX_I2C_WRITE){ //write the remaining bytes that are left off from page write

      write_page(currentSpot,counter);
      currentSpot += counter;    
  }

  Serial.print("Current spot: ");
  Serial.println(currentSpot);

  delay (1000);

  Serial.println();
  Serial.println("(I will start reading what you wrote now)");
  Serial.println();
  Serial.println();
  Serial.println();
  
  delay(3000);

  //READING OPERATION

  if (devices==0){ //if only one device used, read form that device

    for (long x=0; x<currentSpot; x++){
      
        byte val = read_data(x);
        Serial.write(val);
        
      }
    
  }


  else {

    for (int i=0; i<devices; i++){ //read from all but the last device all the MAX_I2C_WRITE bytes
      
      EEPROM_ADR = addresses[i];
      
      for (long x=0; x<MAX_ADR; x++){
      
        byte val = read_data(x);
        Serial.write(val);
        
      }
  }
  
    if (currentSpot!=0){ //read the remaining bytes from the last device
  
      EEPROM_ADR = addresses[devices];
      
      for (long x=0; x<currentSpot; x++){
      
        byte val = read_data(x);
        Serial.write(val);
        
      }
    }
  }
}

void loop()
{
    // Don't do anything here
}

void write_page(long eeAddress, long write_to)
{
  Wire.beginTransmission(EEPROM_ADR);
  Wire.write((int)(eeAddress >> 8)); // MSB
  Wire.write((int)(eeAddress & 0xFF)); // LSB

  //Write bytes to EEPROM
  for (byte x = 0 ; x < write_to ; x++)
    Wire.write(tempStore[x]); //Write the data
  
  Wire.endTransmission(); //Send stop condition
}

byte read_data(long eeaddress)
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

int devices_finder()
{

  //I2C_Scanner that stores the devices in an array
  
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

      addresses[nDevices]=address;
 
      ++nDevices;
      
    }
  }
}
