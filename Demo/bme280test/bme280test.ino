/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution


  Arduino IDE 1.8.6
  NodeMCU V3 - ESP-12E
  http://arduino.esp8266.com/staging/package_esp8266com_index.json

  Andreas Meier - 10.11.2018
   
 ***************************************************************************/

/*-----( Import needed libraries )-----*/
#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <math.h>


// ddmmyy.ver.name
#define SOFTWAREVERSION ("101118.007.am")

#define LED_D0 16 // NodeMCU - D0 ( GPIO 16 - D0 )

/*
define LED_D0 0 // NodeMCU - D0 ( GPIO 16 - D0 )
define LED_D1 5 // NodeMCU - D1 ( GPIO 5  - D1 )
define LED_D2 4 // NodeMCU - D2 ( GPIO 4  - D2 )

default I2C Ports of <Wire.h> 
GPIO 5  - D1 is SDA
GPIO 4  - D2 is SCL

NodeMCU - D1 ( GPIO 5  - D1 - SCL)
NodeMCU - D2 ( GPIO 4  - D2 - SDA)
NodeMCU - D5 ( GPIO 14 - D5 )
NodeMCU - D6 ( GPIO 12 - D6 )
NodeMCU - D7 ( GPIO 13 - D7 )
NodeMCU - D8 ( GPIO 15 - D8 )

*/

//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10

// I2C ADDRESS of BME280 Sensor
// Addafruit default is (0x77)
#define BME280_ADDRESS  (0x76)
    
// define SeaLevelPresure value hPa    
#define SEALEVELPRESSURE_HPA (1013.25)

/*-----( Declare objects )-----*/

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

/*-----( Declare global variables )-----*/

unsigned long delayTime;

// Taupunkt online Rechner
// https://rechneronline.de/barometer/taupunkt.php

// https://www.mikrocontroller.net/topic/415860

float La, RH, TP, E, H, T, pNN, pH, b;
int LED_D0_State;


void setup() {
    Serial.begin(19200);

    // while the serial stream is not open, do nothing:
    while (!Serial) ;

/*-----( Declare Header )-----*/

    Serial.println();
    Serial.println();

    Serial.print("Filename: ");
    Serial.println(__FILE__);
    
    Serial.print("Compilation timestamp: ");
    Serial.println(__DATE__ " " __TIME__);

    Serial.print("Compiler version: ");
    Serial.println(__VERSION__);

    Serial.print("Software version: ");
    Serial.println(SOFTWAREVERSION);

    Serial.print("Debug Line: ");
    Serial.println(__LINE__);
    
    Serial.println("_________________________________________");

/*-----( Declare Header )-----*/

    pinMode(LED_D0, OUTPUT);
    
    Serial.println();
    Serial.println("Start up the <Wire.h> (I2C) library ... ");
    Wire.begin(4, 5);       // sda, scl - with sda cable connected to D2(4) and scl cable connected to D1(5).
    //Wire.begin(14, 12);    // sda, scl - with sda cable connected to D5(14) and scl cable connected to D6(12).

    I2C_Scanner();      // Scan I2C Bus ...
    delay(10);

    Serial.println();
    Serial.println(F("BME280 test"));

    bool status;
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin(BME280_ADDRESS);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("BME280 sensor ready");

    
    Serial.println();    
    Serial.print(__LINE__);
    Serial.println(" --- Init Done !  ---------------------------");
    Serial.println();
    
    delayTime = 5000;

}



// --- Main Cycle -------------------------------------
void loop() { 

// Toggle Led
//    LED_D0_State = !LED_D0_State;         // invert LED_D0_State
//    digitalWrite(LED_D0 , LED_D0_State);  // write state to output

// Blink Led
    digitalWrite(LED_D0 , 1);  // write state to output
    delay(100);
    digitalWrite(LED_D0 , 0);  // write state to output

    printValues();
    delay(delayTime);

}
// --- End of Main Cycle ------------------------------




// --- Sub Functions ------------------------------------
void printValues() { 
   
    T = bme.readTemperature(); 
    Serial.print("Temperature = ");
    Serial.print(T,2);
    Serial.println(" °C");

    Serial.print("Pressure    = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("approx.  Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    RH = bme.readHumidity();
    Serial.print("relativ  Humidity = ");
    Serial.print(RH,2);
    Serial.println(" %rH");

    // E (hPa) ist der Wasserdampfpartialdruck - Magnusformel
    // https://de.wikipedia.org/wiki/S%C3%A4ttigungsdampfdruck

    E = 6.112*exp((17.62*T)/(243.12+T));
    

    La = RH*E/(461.51*(273.15+T))*1000.0;

    Serial.print("absolut  Humidity = ");
    Serial.print(La,2);
    Serial.println(" g/m³");

    // Taupunkt 
    TP=243.12*((17.62*T)/(243.12+T)+log(RH/100.0))/((17.62*243.12)/(243.12+T)-log(RH/100.0));
    
    Serial.print("Dewpoint = ");
    Serial.print(TP, 2);
    Serial.println(" °C");    
    
    Serial.println("---------------------------------------------------------------------");
    Serial.println();

}
