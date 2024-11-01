#include <FastLED.h>

#define NUM_LEDS 5       // Number of LEDs in your strip
#define DATA_PIN 2      // Data pin connected to the LED strip
#define BRIGHTNESS 255  // Initial brightness level (0-255)

#define R1 14 //relay
#define Buzz 16 // Buzzer
#define Btn 15 // Button


CRGB leds[NUM_LEDS];     // Define the LED array

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // Initialize LED strip
  FastLED.setBrightness(BRIGHTNESS);  // Set initial brightness
  pinMode(R1, OUTPUT);
  pinMode(Buzz, OUTPUT); 
  pinMode(Btn, INPUT); 
  digitalWrite(R1, HIGH);
  
  digitalWrite(R1, HIGH);
  
  delay(1000); 
  Serial.begin(115200);
}

void loop() {

   rainbowCycle(1);  // Change 20ms per step


   int buttonState = digitalRead(Btn); // Read the state of the button

  if (buttonState == HIGH) {
      Serial.print("High");
      digitalWrite(Buzz, HIGH); 
      delay(200);               
      digitalWrite(Buzz, LOW); 
      delay(100); 
      digitalWrite(Buzz, HIGH);
      delay(200);               
      digitalWrite(Buzz, LOW);  
      delay(100); 
      digitalWrite(Buzz, HIGH);
      delay(200);               
      digitalWrite(Buzz, LOW);

      Serial.print("LOw");
      

            digitalWrite(R1, HIGH); 
            delay (200);
            digitalWrite(R1, LOW);
   
  } else {
            digitalWrite(R1, LOW);
  }

//  digitalWrite(Buzz, HIGH); 
//  Serial.print("High");
//  delay(2000);               
//  digitalWrite(Buzz, LOW);
//  Serial.print("LOw");
//  delay(2000);

//  rainbowCycle(1);  // Change 20ms per step

//  digitalWrite(R1, HIGH); 
//  Serial.print("High");
//  delay(2000);               
//  digitalWrite(R1, LOW);
//  Serial.print("LOw");
//  delay(2000);   
  // You can add your own patterns or effects here
}

// Function to display a rainbow cycle pattern on the LED strip
void rainbowCycle(uint8_t wait) {
  for (int j = 0; j < 256 * 1; j++) { // 5 cycles of all colors on wheel
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
    }
    FastLED.show();
    delay(wait);
  }
}

// Function to generate rainbow colors
CRGB Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return CRGB(0, 0, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
