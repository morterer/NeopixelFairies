// LED faries that use the NeoPixels library
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 0
#define NUM_PIXELS 14
#define MAX_GLOWING_PIXELS 5

// set up the LED strip
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// array that holds an index for the pixels in leds[] that are lit up
byte glowingPixels[MAX_GLOWING_PIXELS] = {'\0', '\0', '\0', '\0'};

void setup() {
//  Serial.begin(57600);
  ledStrip.begin();
  ledStrip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));
}

void loop() {
  // loop through the glowingPixels array that represents active pixels
  for (int i = 0; i < MAX_GLOWING_PIXELS; i++) {
//    Serial.print("glowingPixels[i] "); Serial.println(glowingPixels[i]);
    // if glowingPixels[i] = '\0' then add a lit pixel
    // and a little bit of random chance
    if (glowingPixels[i] == '\0' && random(100) < 10) {
//      Serial.println("Found an unlit pixel");
      // find a random unused pixel and then
      // assign the index of the pixel to glowingPixels[i]
      glowingPixels[i] = findUnusedPixel();
      // light it up!
      ledStrip.setPixelColor(glowingPixels[i], colorWheel(random(255)));
    } else {
      // else the pixel is active
      // if the brightness of the pixel is 0
      if (ledStrip.getPixelColor(glowingPixels[i]) == 0) {
        // remove it from glowingPixels by setting glowingPixels[i] = -1
//        Serial.println("Removing dark pixel");
        glowingPixels[i] = '\0';
      } else {
        // else fade the pixel
//        Serial.println("Dimming a pixel");
        ledStrip.setPixelColor(glowingPixels[i], dimPixel(ledStrip.getPixelColor(glowingPixels[i]), random(175, 255)));
      }
    }
  }
  ledStrip.show();
  delay(100);
}

uint32_t colorWheel(byte wheelPos) {
  //  wheelPos = 255 - wheelPos;
  if (wheelPos < 85) {
    return ledStrip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170) {
    wheelPos -= 85;
    return ledStrip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ledStrip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// takes packed 32-bit RGB value
uint32_t dimPixel(uint32_t packedColor, uint8_t scale) {
  // unpack all the colors
//  int  r = (uint8_t)(packedColor >> 16);
//  int  g = (uint8_t)(packedColor >>  8);
//  int  b = (uint8_t)packedColor;
//  Serial.print("Old "); Serial.print(r); Serial.print(":"); Serial.print(g); Serial.print(":"); Serial.println(b);

  //  if r is already at zero leave it at zero, otherwise dim the r value
//  r = (r == 0) ? 0 : (((int)r * (int)(scale) ) >> 8);
//  g = (g == 0) ? 0 : (((int)g * (int)(scale) ) >> 8);
//  b = (b == 0) ? 0 : (((int)b * (int)(scale) ) >> 8);
  // r = (((uint16_t)r) * scale_fixed) >> 8;
  //
//  Serial.print("new "); Serial.print(r); Serial.print(":"); Serial.print(g); Serial.print(":"); Serial.println(b); Serial.println("-----");
//  return ledStrip.Color(r, g, b);

  return ledStrip.Color(
    ((uint8_t)(packedColor >> 16) == 0) ? 0 : (((uint8_t)(packedColor >> 16) * (uint8_t)(scale) ) >> 8), 
    ((uint8_t)(packedColor >>  8) == 0) ? 0 : (((uint8_t)(packedColor >>  8) * (uint8_t)(scale) ) >> 8),
    ((uint8_t)packedColor == 0)         ? 0 : (((uint8_t)packedColor         * (uint8_t)(scale) ) >> 8)
  );
}

byte findUnusedPixel() {
  byte randNum = '\0';
  // try and get clever to save a little memory
  while (randNum == '\0') {
    // try another random number
    randNum = random(NUM_PIXELS);
    // loop through all the active pixels to see if randNum is in use
    for (int i = 0; i < MAX_GLOWING_PIXELS; i++) {
      if (glowingPixels[i] == randNum) {
        // yup already used
        randNum = '\0';
        break;
      }
    }
  }
  return randNum;
}
