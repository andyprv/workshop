/*
----------------------------------------------------------------------
I2C-Scanner - with scanblock diagram

Code fuer Arduino
Author Retian
Version 1.0

Der Sketch testet die Standard-7-Bit-Adressen von 1 bis 128
mit den Frequenzen 100 kHz und 400 kHz

ToDo: 
- add scanblock diagram - done

more Infos: https://learn.adafruit.com/i2c-addresses/the-list
more Infos: https://arduino-projekte.webnode.at/i2c-scanner/

Andreas Meier - 11.11.2018

----------------------------------------------------------------------
*/

void I2C_Scanner()
{
  Serial.println("\nI2C-Scanner - with scanblock diagram");

  byte error, address;
  int nDevices;

  Serial.println("Scanning ... ");
  
  // disabeld, cause main programm decide the i2c bus speed !
  //Serial.println("Scanning (100 kHz) ... ");
  //Wire.setClock(100000L);

  //Serial.println("Scanning (400 kHz) ... ");
  //Wire.setClock(400000L);

  // print help for I2C status  !
  Serial.println();
  Serial.println("  ! I2C device found, no error detected");
  Serial.println("  e I2C device found, but Unknow error detected");
  Serial.println("--- I2C no device found");
  Serial.println();
  
  // block diagram write header line
  Serial.println("     0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f");
  Serial.print("00: --- ");
  
  nDevices = 0;
  for(address = 1; address < 128; address++ ) 
  {
    
    //delay(500);

    // The i2c_scanner uses the return value of
    // the Write.end Transmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      
      //Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.print("! ");

      nDevices++;
    }
    else if (error==4) 
    {
      //Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.print("e ");
    }    
    else
    {
        Serial.print("--- ");
    }
  
      switch (address) {
        case 15: 
          //do something when var equals 15 (0x0F)
          Serial.println();
          Serial.print("10: ");
          break;
        case 31:
          //do something when var equals 30 (0x1F)
          Serial.println();
          Serial.print("20: ");
          break;
        case 47:
          //do something when var equals 47 (0x2F)
          Serial.println();
          Serial.print("30: ");
          break;
        case 63:
          //do something when var equals 63 (0x3F)
          Serial.println();
          Serial.print("40: ");
          break;
        case 79:
          //do something when var equals 79 (0x4F)
          Serial.println();
          Serial.print("50: ");
          break;
        case 95:
          //do something when var equals 95 (0x5F)
          Serial.println();
          Serial.print("60: ");
          break;
        case 111:
          //do something when var equals 111 (0x6F)
          Serial.println();
          Serial.print("70: ");
          break;
                 
        } // end switch case
  
  } // end for


  // end for loop
  Serial.println();
  
  if (nDevices == 0)
    while(1)
    {     
      Serial.println("No I2C devices found !  ... please Restart ");
      delay(1000);
    }   
  else
    Serial.println("done\n");

}


// -----------------------------------------------------------------------


