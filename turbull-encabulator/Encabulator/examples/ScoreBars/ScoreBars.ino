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

void loop() {
   
  for (i = 0; i < 10; i++) {
      r = random(255);
      g = random(255);
      b = random(255);

      // score 1 - 8
      for (j = 1; j < 9; j++) {
         Encabulator.lightUpBars(j,r,g,b);
         delay(2000);
      }
  }

}

