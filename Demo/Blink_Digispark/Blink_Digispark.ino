/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
  
  http://digistump.com/

  In the box labeled “Additional Boards Manager URLs” enter:
  http://digistump.com/package_digistump_index.json

  How to Start
  http://digistump.com/wiki/digispark/tutorials/connecting
  https://www.electroschematics.com/12102/learn-to-use-attiny85-usb-mini-development-board/
 
  Connecting ATTiny 
  https://www.youtube.com/watch?v=MmDBvgrYGZs

  Pin outs:  All pins can be used as Digital I/O
  
  Pin 0 → I2C SDA, PWM (LED on Model B)
  Pin 1 → PWM (LED on Model A)
  Pin 2 → I2C SCK, Analog In
  Pin 3 → Analog In (also used for USB+ when USB is in use)
  Pin 4 → PWM, Analog (also used for USB- when USB is in use)
  Pin 5 → Analog In

  Hints !
  http://digistump.com/wiki/digispark/tutorials/basics


  Pin 3 and Pin 4 (P3 and P4) are used for USB communication and programming, 
        while you can use them in your circuit if you are not using USB communication, 
        you may have to unplug your circuit during programming if the circuit would impede 
        the pin states or dramatically affect the voltage levels on these pins.
  
  Pin 3 (P3) has a 1.5 kΩ pull-up resistor attached to it which is required for 
        when P3 and P4 are used for USB communication (including programming). 
        Your design may need to take into account that you'd have to overpower this to pull this pin low.


*/

// ddmmyy.ver.name
#define SOFTWAREVERSION ("171118.001.am")

/*-----( Import needed libraries )-----*/
//#include <Wire.h>
//#include <SPI.h>
//#include <math.h>

// EEPROM config
//#include <EEPROM.h>

#define LED_BUILTIN   1   // Digispart ATTiny - D0 ( GPIO 16 - D0 )


 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                       // wait for a second
}
