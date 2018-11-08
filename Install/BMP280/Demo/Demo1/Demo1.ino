/*
Beispiel 1:

Im Beispiel 1 werden die Messwerte im Normal-Modus mit den in der Library 
voreingestellten Parametern zyklisch berechnet und ausgegeben. 
Nur der Parameter für Druck-Oversampling wird geändert. 
Der voreingestellte Wert für die Standby-Zeit ist 1000 ms. 
Das heißt, dass ca. einmal pro Sekunde eine neue Messung vom Sensor 
durchgeführt wird und Temperatur und Druck als Rohwerte gespeichert werden. 
Daher lese ich auch nur einmal pro Sekunde die Werte aus.

Testprogramm 1 für Luftdrucksensor BMP280
Code fuer Arduino
Author Retian
Version 1.0

*/

#include <MyBMP280.h>
//#include <Metro.h>

MyBMP280 bmp(0x76);
//Metro leseZyklus = Metro(1000); //Zykluszeit in ms für Auslesen des Sensors

int hoehe = 460; //Messort: 460 m über dem Meer
float luftTemp, luftDruck, redLuftDruck;

void setup() {
  Serial.begin(9600);
  if (bmp.isReady()) Serial.println("BMP280 Ok!");
  else
  {
    Serial.println("BMP280 Fehler!");
    while (1); //Im Fehlerfall geht's hier nicht weiter!
  }

  bmp.setPressOversampling(P_OVERSAMPLING_x2); //Ändere Druck-Oversampling Parameter
  bmp.init();
}



void loop() {
    luftTemp = bmp.readTemp();
    Serial.print("Lufttmperatur  : ");
    Serial.print(luftTemp);
    Serial.println(" Grad C");

    luftDruck = bmp.readPress();
    Serial.print("Luftdruck      : ");
    Serial.print(luftDruck);
    Serial.println(" hPa");

    redLuftDruck = bmp.readReducedPress(hoehe);
    Serial.print("Reduz.Luftdruck: ");
    Serial.print(redLuftDruck);
    Serial.println(" hPa");

    Serial.println();
}
