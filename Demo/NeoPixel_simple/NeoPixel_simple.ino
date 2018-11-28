// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            D1

int led = D0;          // the PWM pin the LED is attached to

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      3 //61

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 25; // delay for half a second

void setup() 
{
  Serial.begin(19200);             // baudrate for serial Logger --> 19200

  pinMode(led, OUTPUT);

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();  // This sends the updated pixel color to the hardware.

  Serial.println("setup DONE ....");
  delay(1000);

}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for(int i=0;i<( NUMPIXELS/2 );i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor((23-i), pixels.Color(255,128,0)); // Moderately bright green color.
    pixels.setPixelColor((38+i), pixels.Color(150,150,0)); // Moderately bright green color.

    if ( i>( 9 ) ) {

    pixels.setPixelColor((23-( i-10 ) ), pixels.Color(0,0,0)); // Moderately bright green color.
    pixels.setPixelColor((38+( i-10 ) ), pixels.Color(0,0,0)); // Moderately bright green color.
      
    }


    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

    Serial.println("loop ....");

  }


  for(int i=0;i<NUMPIXELS;i++)
    {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // clear pixels
    
    }

  digitalWrite(led, LOW);
  
  delay(200);
  pixels.show(); // This sends the updated pixel color to the hardware.
  
  delay(200);
  
  digitalWrite(led, HIGH);
    

}
