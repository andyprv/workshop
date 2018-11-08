//MyBMP280.cpp
//Code fuer Arduino
//Author Retian
//Version 3.3

/*
Abfrage eines Luftdrucksensors BMP280 ueber I2C- oder SPI-Schnittstelle.

MyBMP280 Name(i2cAdd);        //fuer I2C-Schnittstelle
MyBMP280 Name(cs, interface); //fuer SPI-Schnittstelle; interface: BMP280_SPI

Beispiel siehe unter:
http://arduino-projekte.webnode.at/meine-libraries/luftdrucksensor-bmp280/

Funktionen siehe unter:
http://arduino-projekte.webnode.at/meine-libraries/luftdrucksensor-bmp280/funktionen/

*/

//*************************************************************************

#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include "MyBMP280.h"

MyBMP280::MyBMP280(byte i2cAdd) : _cs(-1)
{
  _i2cAdd = i2cAdd;
  
  Wire.begin();
  setDefaultParameters();
}

MyBMP280::MyBMP280(byte cs, byte interface)
{
  //"interface" wird nicht benoetigt, ist nur zur Unterscheidung
  //beim Aufruf der Instanzen zwischen SPI- und I2C-Schnittstelle
  _cs = cs;
    
  pinMode(_cs, OUTPUT);
  SPI.begin();
  setDefaultParameters();
}

//*************************************************************************
//Setzen der Standard-Parameter
//(Interne Verwendung)

void MyBMP280::setDefaultParameters()
{
  controlVal = 0;
  configVal = 0;
  
  //Setzen der Voreinstellungen
  mode = NORMAL_MODE;
  setMode(mode);
  
  osrs_p = P_OVERSAMPLING_x8; //Pressure oversampling x8
  setPressOversampling(osrs_p);
  
  osrs_t = T_OVERSAMPLING_x2; //Temperature oversampling x2
  setTempOversampling(osrs_t);
  
  t_sb =  STANDBY_TIME_1000; //Standby time (NORMAL_MODE) = 1000 ms
  setStandbyTime(t_sb);
  
  filter = FILTER_COEFF_4; //Filter-Koeffizient = 4 (5 Stufen fuer 75%)
  setFilterCoeff(filter);
}

//*************************************************************************
//BMP280 vorhanden ?

bool MyBMP280::isReady()
{
  if (read8(BMP280_register_chip_id) == chip_ID) return true;
  else return false;
}

//*************************************************************************
//BMP280 initialisiern
void MyBMP280::init()
{
  //BMP280 zuruecksetzen
  reset();
  delay(250); 
  
  //Kalibrierungskoeffizienten einlesen
  
  //Wenn der BMP280 gerade die Koeffizienten ins interne
  //Image-Register liest, dann warte
  while (read8(BMP280_register_status) & 0x01) delay(1);
	
  dig_T1 = (unsigned int)read16(BMP280_register_dig_T1);
  dig_T2 = read16(BMP280_register_dig_T2);
  dig_T3 = read16(BMP280_register_dig_T3);
  dig_P1 = (unsigned int)read16(BMP280_register_dig_P1);
  dig_P2 = read16(BMP280_register_dig_P2);
  dig_P3 = read16(BMP280_register_dig_P3);
  dig_P4 = read16(BMP280_register_dig_P4);
  dig_P5 = read16(BMP280_register_dig_P5);
  dig_P6 = read16(BMP280_register_dig_P6);
  dig_P7 = read16(BMP280_register_dig_P7);
  dig_P8 = read16(BMP280_register_dig_P8);
  dig_P9 = read16(BMP280_register_dig_P9);
	   
  //Schreiben der gesetzten Parameter in die Register 
  write8(BMP280_register_config, configVal);
  write8(BMP280_register_control, controlVal);
}

//*************************************************************************
//Starte Einzelmessung (FORCED_MODE)

void MyBMP280::startSingleMeas()
{
  controlVal &= B11111100;
  controlVal |= FORCED_MODE;
  write8(BMP280_register_control, controlVal);
  while (statusMeas()); //Warte bis Messzyklus beendet
}

//*************************************************************************
//Ermittlung der Lufttemperatur

float MyBMP280::readTemp()
{
  readRawData();
  if (adc_T == 0x80000)  //Rueckgabe, wenn Temperaturmesseung ausgeschaltet ist
	return NAN;
  return calcTemp();
}

//*************************************************************************
//Ermittlung des Luftdrucks

float MyBMP280::readPress()
{
  float press;
  
  readRawData();
  if (adc_P == 0x80000)  //Rueckgabe, wenn Druckmesseung ausgeschaltet ist
	return NAN;
  calcTemp(); //calcPress() benoetigt t_fine aus calcTemp()
  return calcPress();
}

//*************************************************************************
//Reduzierten Luftdruck bezogen auf Meereshoehe ermitteln

float MyBMP280::readReducedPress(int hoehe)
{
  //Berechne reduzierten Luftdruck
  int _hoehe = hoehe;
  readRawData();
  if (adc_P == 0x80000) //Rueckgabe, wenn Luftdruckmessung ausgeschaltet ist
	return NAN;
  calcTemp();
  return (calcPress() / (pow(1.0 - ((float)hoehe / 44330.0), 5.255)));
 }

//*************************************************************************
//Einlesen von 6 Bytes (burst read) ueber die I2C-Schnittstelle
//(Interne Verwendung)

void MyBMP280::readRawData()
{
  while (statusMeas()); //Warte bis Messzyklus beendet
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(BMP280_register_data);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAdd, (byte)6);
    adc_P = Wire.read();
    adc_P <<= 8;
    adc_P |= Wire.read();
    adc_P <<= 8;
    adc_P |= Wire.read();
    adc_P >>= 4;
    adc_T = Wire.read();
    adc_T <<= 8;
    adc_T |= Wire.read();
    adc_T <<= 8;
    adc_T |= Wire.read();
    adc_T >>= 4;
  }
  
  else
  {
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    SPI.transfer(BMP280_register_data | 0x80);
    adc_P = SPI.transfer(0);
    adc_P <<= 8;
    adc_P |= SPI.transfer(0);
    adc_P <<= 8;
    adc_P |= SPI.transfer(0);
    adc_P >>= 4;
    adc_T = SPI.transfer(0);
    adc_T <<= 8;
    adc_T |= SPI.transfer(0);
    adc_T <<= 8;
    adc_T |= SPI.transfer(0);
    adc_T >>= 4;
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();
  }
}

//*************************************************************************
//Temperatur ermitteln
//(Interne Verwendung)

float MyBMP280::calcTemp()
{
  int32_t var1, var2;
  int32_t temp;

  //Berechne Temperatur
  var1 = (((adc_T >> 3) - (dig_T1 << 1)) * dig_T2) >> 11;
  var2 = (((((adc_T >> 4) - (dig_T1)) * ((adc_T >> 4) - dig_T1)) >> 12) * dig_T3) >> 14;

  t_fine = var1 + var2;
  temp = (t_fine * 5 + 128) >> 8;
  return (float)temp / 100.0;
}

//*************************************************************************
//Luftdruck ermitteln
//(Interne Verwendung)

float MyBMP280::calcPress()
{
  int64_t var1, var2;
  int64_t press;
  
  //Berechne Luftdruck
  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)dig_P6;
  var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
  var2 = var2 + (((int64_t)dig_P4) << 35);
  var1 = ((var1 * var1 + (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
  if (var1 == 0) return 0;
  press= 1048576 - adc_P;
  press= (((press << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)dig_P9) * (press>> 13) * (press>> 13)) >> 25;
  var2 = (((int64_t)dig_P8) * press) >> 19;
  press = ((press + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
  press /= 256;
  return (float) press/ 100.0;
}

//*************************************************************************
// Schreiben von 1 Byte ueber die I2C-Schnittstelle
//(Interne Verwendung)

void MyBMP280::write8(byte add, byte val)
{
  byte _add = add;
  byte _val = val;
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(_add);
    Wire.write(_val);
    Wire.endTransmission();
  }
  
  else
  {
    digitalWrite(_cs, LOW);
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(_add & 0x7F);
    SPI.transfer(val);
    SPI.endTransaction();
    digitalWrite(_cs, HIGH);
  }
  delay(5);
}

//*************************************************************************
// Einlesen von 1 Byte ueber die I2C-Schnittstelle
//(Interne Verwendung)

byte MyBMP280::read8(byte adr)
{
  byte _adr = adr;
  byte val;
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(_adr);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAdd, (byte)1);
    val = Wire.read();
  }
    
  else
  {
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    SPI.transfer(_adr | 0x80);
    val = SPI.transfer(0);
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();
  }
  return val;
}

//*************************************************************************
// Einlesen von 2 Bytes ueber die I2C-Schnittstelle 
//(Interne Verwendung)

int MyBMP280::read16(byte adr)
{
  byte _adr = adr;
  int val;
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(_adr);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAdd, (byte)2);
    val = Wire.read() | (Wire.read() << 8);
  }
  
  else
  {
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    SPI.transfer(_adr | 0x80);
    val = SPI.transfer(0) | (SPI.transfer(0) << 8);
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();
  }
  return val;
}

//*************************************************************************
//Setzen des Modus

void MyBMP280::setMode(byte mode)
{
  byte _mode = mode;
  
  controlVal &= B11111100; //Loesche mode 
  controlVal |= _mode;
}

//*************************************************************************
//Setzen Temperatur-Oversampling

void MyBMP280::setTempOversampling(byte osrs_t)
{
  byte _osrs_t = osrs_t;
    
  controlVal &= ~(B111 << 5); //Loesche Temp.Oversampling
  controlVal |= (_osrs_t << 5);
}

//*************************************************************************
//Setzen Druck-Oversampling

void MyBMP280::setPressOversampling(byte osrs_p)
{
  byte _osrs_p = osrs_p;
  
  controlVal&= ~(B111 << 2); //Loesche Press.Oversampling
  controlVal |= (_osrs_p << 2);
}

//*************************************************************************
//Setzen Standby-Zeit (Normal-Modus)

void MyBMP280::setStandbyTime(byte t_sb)
{
  byte _t_sb = t_sb;
  
  configVal &= ~(B111 << 5); //Loesche Standby-Zeit
  configVal |= (_t_sb << 5);
}

//*************************************************************************
//Setzen des Filterkoeffizienten

void MyBMP280::setFilterCoeff(byte filter)
{
  byte _filter = filter;

  configVal &= ~(B111 << 2); //Loesche Filter
  configVal |= (_filter << 2);
}

//*************************************************************************
//Abfrage des Controll-Register 

byte MyBMP280::readControlReg(void)
{
  return read8(BMP280_register_control);
}

//*************************************************************************
//Abfrage des Config-Register 

byte MyBMP280::readConfigReg(void)
{
  return read8(BMP280_register_config);
}

//*************************************************************************
//Abfrage des Status des Measuring-Bit (interne Verwendung)

bool MyBMP280::statusMeas()
{
  byte reg;
	
  reg = read8(BMP280_register_status);
  return (reg & B00001000) >> 3;
}

//*************************************************************************
//Software-Reset (interne Verwendung)

void MyBMP280::reset()
{
  write8(BMP280_register_reset, RESET);
}

//*************************************************************************
//Voreinstellung fuer Weather Monitoring

void MyBMP280::setWeatherMonitoring()
{
  mode = FORCED_MODE;
  setMode(mode);
  
  osrs_p = P_OVERSAMPLING_x1; //Pressure oversampling x1
  setPressOversampling(osrs_p);
  
  osrs_t = T_OVERSAMPLING_x1; //Temperature oversampling x1
  setTempOversampling(osrs_t);
    
  /*Keine Bedeutung bei WeatherMonitoring (Forced Mode)
  t_sb =  STANDBY_TIME_62p5; //Standby time (NORMAL_MODE) = 62.5 ms
  setStandbyTime(t_sb);
  */
  
  filter = FILTER_COEFF_0; //Filter aus
  setFilterCoeff(filter);
}

//*************************************************************************
//Voreinstellung fuer "handheld device low power"

void MyBMP280::setHandheldLowPower()
{
  mode = NORMAL_MODE;
  setMode(mode);
  
  osrs_p = P_OVERSAMPLING_x16; //Pressure oversampling x16
  setPressOversampling(osrs_p);
  
  osrs_t = T_OVERSAMPLING_x2; //Temperature oversampling x2
  setTempOversampling(osrs_t);
  
  t_sb =  STANDBY_TIME_62p5; //Standby time (NORMAL_MODE) = 62.5 ms
  setStandbyTime(t_sb);
  
  filter = FILTER_COEFF_4; //Filter-Koeffizient = 4 (5 Stufen fuer 75%)
  setFilterCoeff(filter);
}

//*************************************************************************
//Voreinstellung fuer "handheld device dynamic"

void MyBMP280::setHandheldDynamic()
{
  mode = NORMAL_MODE;
  setMode(mode);
  
  osrs_p = P_OVERSAMPLING_x4; //Pressure oversampling x4
  setPressOversampling(osrs_p);

  osrs_t = T_OVERSAMPLING_x1; //Temperature oversampling x1
  setTempOversampling(osrs_t);
  
  t_sb =  STANDBY_TIME_0p5; //Standby time (NORMAL_MODE) = 0.5 ms
  setStandbyTime(t_sb);
  
  filter = FILTER_COEFF_16; //Filter-Koeffizient = 16 (22 Stufen fuer 75%)
  setFilterCoeff(filter);
}
