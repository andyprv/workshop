/*
Programmbeispiele MyBME280 mit I2C- oder SPI-Schnittstelle:

Beispiel 1 für Arduino:

Im Beispiel 1 werden die Parameter des Sensors auf "Weather Monitoring" gesetzt 
und mit der Funktion startSingleMeas() wird alle 60 Sekunden 
eine neue Messung gestartet und die Messergebnisse angezeigt.


Testprogramm 1 für Luftdrucksensor BME280
Code fuer Arduino
Author Retian
Version 1.0

Mehr Infos: https://arduino-projekte.webnode.at/meine-libraries/luftdruck-luftfeuchtesensor-bme280/

*/


//Prototypen:
void leseMesswerte(void);
void ausgabeMesswerte(void);


#include <MyBME280.h>


#define cs 10 //CS-Pin für SPI-Schnittstelle
#define BME280_I2cAdd 0x76 //I2C-Adresse



MyBME280 bme(BME280_I2cAdd); //mit I2C-Schnittstelle oder
//MyBME280 bme(cs, BME280_SPI); //mit SPI-Schnittstelle



unsigned long zeit;

float luftTemp, luftDruck, redLuftDruck, luftFeuchte;
int hoehe = 460; //Messort 460 m ueber dem Meer



void setup() {
  Serial.begin(115200);
  if (bme.isReady())
  {
    Serial.println("BME280 ok");
    bme.setWeatherMonitoring(); //Setze Parameter für Wetterueberwachung
    bme.init();
    //Beim Initialisieren des Parametersatzes für Wetterueberwachung (Forced-Mode)
    //wird bereits eine Messung vom Sensor durchgeführt und als Rohwerte gespeichert
    leseMesswerte();
    ausgabeMesswerte();
  }
  else
  {
    Serial.println("BME280 Fehler");
    while (1); //hier geht es im Fehlerfall nicht weiter
  }
  zeit = millis();
}



void loop() {
  if (millis() > zeit + 60000) //Messung alle 60 Sekunden
  {
    bme.startSingleMeas();
    leseMesswerte();
    ausgabeMesswerte();
    zeit = millis();
  }
}



void leseMesswerte()
{
  luftTemp = bme.readTemp();
  luftDruck = bme.readPress();
  redLuftDruck = bme.readReducedPress(hoehe);
  luftFeuchte = bme.readHumidity();
}



void ausgabeMesswerte()
{
  Serial.print("Lufttemperatur  : ");
  Serial.print(luftTemp);
  Serial.println(" Grad C");
  Serial.print("LuftDruck       : ");
  Serial.print(luftDruck);
  Serial.println(" hPa");
  Serial.print("Red. LuftDruck  : ");
  Serial.print(redLuftDruck);
  Serial.println(" hPa");
  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(luftFeuchte);
  Serial.println(" %");
  Serial.println();
}

