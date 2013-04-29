#include <Encabulator.h>
#include <Wire.h>

int r = 0;
int g = 0;
int b = 0;
int i = 0;
int j = 0;

void setup() {               
  
  Encabulator.upUpDownDownLeftRightLeftRightBA();
  Encabulator.setVerbose(true);

}

void pickRandomColor() {
  int color = random(8);

  r = 0;
  g = 0;
  b = 0;

  if (color == 0) { // red
    r = 255;
  } else if (color == 1) { // orange
    r = 255;
    g = 128;
  } else if (color == 2) { // yellow
    r = 255;
    g = 255;
  } else if (color == 3) { // green
    g = 255;
  } else if (color == 4) { // teal
    g = 128;
    b = 255;
  } else if (color == 5) { // blue
    b = 255;
  } else if (color == 6) { // magenta
    r = 204;
    b = 204;
  } else if (color == 7) { // violet
    r = 127;
    b = 255;
  } else { // white
    r = 255;
    g = 255;
    b = 255;
  }
}

void loop() {

  // demo all win conditions

  for (j = 1; j < 9; j++) {
    pickRandomColor();
    Encabulator.lightUpBars(j,r,g,b);
    delay(2000);
  }

  for (j = 1; j < 9; j++) {
    pickRandomColor();
    Encabulator.pulseBars(j,r,g,b);
    delay(2000);
  }

  for (j = 1; j < 9; j++) {
    Encabulator.rainbowBars(j);
    delay(2000);
  }
}

