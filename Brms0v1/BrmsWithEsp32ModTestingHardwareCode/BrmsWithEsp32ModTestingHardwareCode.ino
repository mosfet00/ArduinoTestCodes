#include <FastLED.h>
 
#define LED_PIN     13
#define NUM_LEDS    2
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
 
#define UPDATES_PER_SECOND 100
 

 
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
 
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
 



// Define the GPIO pins for sensors, button, and buzzer




const int sensorPins[] = {14, 15, 16, 17, 18, 23, 25, 19, 27, 32, 33, 34, 35, 36, 39, 4};
const int buttonPin = 4;
// const int buzzerPin = 2
const int buzzer = 2;
const int f = 1000;





void setup() {
  // Initialize serial communication
  Serial.begin(115200);

    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
   
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

  // Set sensor pins as input
  for (int i = 0; i < 16; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  // Set button pin as input
  pinMode(buttonPin, INPUT);

  // // Set buzzer pin as output
  // pinMode(buzzerPin, OUTPUT);

  pinMode(buzzer, OUTPUT);  // Buzzer
}

void loop() {
  // Check the state of each sensor and print to Serial Monitor

  for (int i = 0; i < 16; i++) {
    int sensorState = digitalRead(sensorPins[i]);
    Serial.print(" (GPIO ");
    Serial.print(sensorPins[i]);
    Serial.print(")>>---->(Sensor ");
    Serial.print(i + 1);
    Serial.print("): ");
    Serial.println(sensorState == HIGH ? "HIGH" : "LOW");
  }

  // Check the state of the button
  int buttonState = digitalRead(buttonPin);

  // If the button is pressed, activate the buzzer
  if (buttonState == HIGH) {
    // digitalWrite(buzzerPin, HIGH);
    tone(buzzer, f);  // turn the LED on (HIGH is the voltage level)
  } else {
    // digitalWrite(buzzerPin, LOW);
    noTone(buzzer);   // turn the LED off by making the voltage LOW
  }

  // Add a small delay to avoid flooding the Serial Monitor
  delay(300);


    ChangePalettePeriodically();
   
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
   
    FillLEDsFromPaletteColors( startIndex);
   
    FastLED.show();
    FastLED.delay(50 / UPDATES_PER_SECOND);
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
   
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
 
 
// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.
 
void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
   
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}
 
// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}
 
// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to w hite.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
   
}
 
// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
   
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}
 
 
// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
   
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
   
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
 


  
