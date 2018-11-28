/*
  Fade

  This example shows how to fade an LED on pin 9 using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Fade
*/

// Variables will change:
int led = D1;          // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
  Serial.begin(19200);
}

// the loop routine runs over and over again forever:
void loop() {

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);

  // brightness = 255 - sensorValue;

  // read the pushbutton input pin:
  buttonState = (sensorValue > 200);
  
  brightness = (buttonState*800);

  // set the brightness of pin 9:
  analogWrite(led, brightness);


/*
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 254) {
    fadeAmount = -fadeAmount;
  }
*/

  
  // print out the value you read:
  Serial.print("sensorValue: ");
  Serial.print(sensorValue);

  Serial.print(" , ");

  Serial.print("brightness: ");
  Serial.print(brightness);

  Serial.print(" , ");

  Serial.print("buttonState: ");
  Serial.println( (buttonState*800) );

  
  // wait for 30 milliseconds to see the dimming effect
  delay(10);

  
}
