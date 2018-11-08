//MyBMP280.h

#ifndef MyBMP280_h
#define MyBMP280_h

#include "Arduino.h"

#define BMP280_SPI 1 //SPI-Schnittstelle

#define BMP280_register_chip_id 0xD0
#define chip_ID 0x58

#define BMP280_register_dig_T1 0x88
#define BMP280_register_dig_T2 0x8A
#define BMP280_register_dig_T3 0x8C
#define BMP280_register_dig_P1 0x8E
#define BMP280_register_dig_P2 0x90
#define BMP280_register_dig_P3 0x92
#define BMP280_register_dig_P4 0x94
#define BMP280_register_dig_P5 0x96
#define BMP280_register_dig_P6 0x98
#define BMP280_register_dig_P7 0x9A
#define BMP280_register_dig_P8 0x9C
#define BMP280_register_dig_P9 0x9E

#define BMP280_register_temp_msb 0xFA
#define BMP280_register_press_msb 0xF7

#define BMP280_register_reset 0xE0
#define BMP280_register_status 0xF3
#define BMP280_register_control 0xF4
#define BMP280_register_config 0xF5
#define BMP280_register_data 0xF7

#define NORMAL_MODE B11
#define FORCED_MODE B01
#define SLEEP_MODE B00

#define RESET 0xB6

#define T_OVERSAMPLING_x0 B000 
#define T_OVERSAMPLING_x1 B001 
#define T_OVERSAMPLING_x2 B010
#define T_OVERSAMPLING_x4 B011
#define T_OVERSAMPLING_x8 B100
#define T_OVERSAMPLING_x16 B101

#define P_OVERSAMPLING_x0 B000
#define P_OVERSAMPLING_x1 B001 
#define P_OVERSAMPLING_x2 B010
#define P_OVERSAMPLING_x4 B011
#define P_OVERSAMPLING_x8 B100
#define P_OVERSAMPLING_x16 B101

#define STANDBY_TIME_0p5 B000
#define STANDBY_TIME_62p5 B001
#define STANDBY_TIME_125 B010
#define STANDBY_TIME_250 B011
#define STANDBY_TIME_500 B100
#define STANDBY_TIME_1000 B101
#define STANDBY_TIME_2000 B110
#define STANDBY_TIME_4000 B111

#define FILTER_COEFF_0 B000
#define FILTER_COEFF_2 B001
#define FILTER_COEFF_4 B010
#define FILTER_COEFF_8 B011
#define FILTER_COEFF_16 B100



class MyBMP280
{
  public:
    MyBMP280(byte);
    MyBMP280(byte, byte);
    bool isReady(void);
    void init();
    void startSingleMeas(void);
    float readTemp(void);
    float readPress(void);
    float readReducedPress(int);
    void setMode(byte);
    void setTempOversampling(byte);
    void setPressOversampling(byte);
    void setStandbyTime(byte);
    void setFilterCoeff(byte);
    byte readControlReg(void);
    byte readConfigReg(void);
    void setWeatherMonitoring(void);
    void setHandheldLowPower(void);
    void setHandheldDynamic(void);
     
  private:
    void readRawData(void);
    float calcTemp(void);
    float calcPress(void);
    byte read8(byte);
    int read16(byte);
    void write8(byte, byte);
    bool statusMeas(void);
    void reset(void);
    void setDefaultParameters();
    
    unsigned int dig_T1;
    int dig_T2;
    int dig_T3;
    unsigned int dig_P1;
    int dig_P2;
    int dig_P3;
    int dig_P4;
    int dig_P5;
    int dig_P6;
    int dig_P7;
    int dig_P8;
    int dig_P9;
    long t_fine;
    long adc_T;
    long adc_P;
  
    byte _i2cAdd;
    int8_t _cs;

    byte mode;
    byte osrs_t;
    byte osrs_p;
    byte t_sb;
    byte filter;
   
    byte controlVal;
    byte configVal;
    
};

#endif

