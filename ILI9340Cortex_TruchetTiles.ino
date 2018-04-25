/************************************************************************
  Truchet Tiles 
  Adafruit ILI9341 and Feather MO Cortex
  Paul J Karlik | pjkarlik@gmail.com | http://github.com/pjkarlik

  Truchet tiles are square tiles decorated with patterns that are not
  rotationally symmetric. When placed within a square tiling of the plane,
  they can form varied patterns, and the orientation of each tile can
  be used to visualize information associated with the tile's position.
  
  This Arduino version is based off my GLSL Shader code
  code writting in JavaScript. 
 *************************************************************************/
 
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "math.h"

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int range = 30;                 // the value used to determin one quadrant
int h = 320 / range;            // the width in quadrant sizes
int w = 240 / range;            // the height in quadrant sizes
float hr = range / 2;           // the offset based on quadrant size
float df;                       // the difference float to render display

const int numReadings = 10;           // the sample index range for reading
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup(void) {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  randomSeed(analogRead(0));
}

void loop() {
  for(int x = 0; x < w; x++){
    for(int y = 0; y < h; y++){
      float check = random(1,4);
      drawTile(x, y, check);
    }
    // the analog read happens every row - can move based on need of refresh
    readPins();
  }
  delay(3000);
}

// Altered from Arduino tutorial --> https://www.arduino.cc/en/Tutorial/Smoothing
void readPins() {
  for(int readIndex = 0; readIndex < numReadings; readIndex++){
    // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = floor(analogRead(14) * 0.2);
    // add the reading to the total:
    total = total + readings[readIndex];
    // calculate the average:
    average = total / numReadings;
  }
  // if the value isn't equal to by more than 2 +/- then set new range size for quadrant
  if(range != average && range < average - 2 || range > average + 2) {
    range = average;
    h = 320 / range;
    w = 240 / range;
    hr = range / 2;
    tft.fillScreen(ILI9341_BLACK);
  }
}

void drawTile(int x, int y, int check) {
  
  // loop over quadrants width and height
  for(int dx = 0; dx < range; dx++){
    for(int dy = 0; dy < range; dy++){

      // Use check to draw tile rotated by formulas
      switch(check) {
        case 1:
          df = dx - dy;
          break;
        case 2:
          df = range - dy - dx;
          break;
        case 3:
          df = dy - dx;
          break;
        case 4:
          df = dy - range + dx;
          break;
        default:
          df = 0;
          break;
       } 

      // set color to white or black only 
      uint16_t flop = df > .5 ? 255 : 0;
      
      tft.fillRect(
        (x * range) + dx,
        (y * range) + dy,
        1,
        1,
        tft.color565(flop, flop, flop)
      );  
    }
  }
}
 

