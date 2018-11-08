/*
Programmbeispiel MyAttinyBMP280

Nachfolgendes Programmbeispiel gibt die gemessenen bzw. berechneten Werte 
für Temperatur, Luftdruck und reduzierten Luftdruck auf einer 
4-stelligen 7-Segmentanzeige mit I2C-Schnittstelle aus. 
Die Ausgabe erfolgt bei mir auf einer Anzeige von Sparkfun (Hersteller-Nr. COM-1144x), 
für die ich auch eine Library geschrieben habe.

Die Library kann hier heruntergeladen werden: 7-Segm.anz. für Attiny (2).

Testprogramm für Luftdrucksensor BMP280
Code fuer Attiny
Author Retian
Version 1.0

*/

//#include <MyAttinySparkfun7SegI2C.h>
#include <MyAttinyBMP280.h>

MyAttinyBMP280 bmp(0x77);
//MyAttinySparkfun7SegI2C seg7(0x71);

int hoehe = 460; //Standorthoehe ueber dem Meer



void setup() {
  if (bmp.isReady()) bmp.init();
  else while(1); //Im Fehlerfall geht's hier nicht weiter!
}



void loop() {
  //Anzeige der Temperatur
  float temp = bmp.readTemp();
  //seg7.sendFloatVal(temp);
  delay(2000);
  //Anzeige des Luftdrucks
  float pressure = bmp.readPress();
  //seg7.sendFloatValPos(pressure);
  delay(2000);
  //Anzeige des reduzierten Luftdrucks
  float redPress = bmp.readReducedPress(hoehe);
  //seg7.sendFloatValPos(redPress);
  delay(2000);
}
