#include <Encabulator.h>
#include <Wire.h>

int ra = 0;
int ga = 0;
int ba = 0;
int rb = 0;
int gb = 0;
int bb = 0;
int i = 0;

void setup() {               
  
  Encabulator.upUpDownDownLeftRightLeftRightBA();
  Encabulator.setVerbose(true);

}

void loop() {
   
  for (i = 0; i < 10; i++) {
      ra = random(255);
      ga = random(255);
      ba = random(255);
      rb = random(255);
      gb = random(255);
      bb = random(255);

      // random gradient
      Encabulator.addressable.drawGradient(ra,ga,ba,rb,gb,bb,64);   
      delay(1000);
  }

  Encabulator.addressable.drawGradient(0,0,0,0,0,0,64);   

  for (i = 0; i < 4; i++) {
      // color-changing comet
      Encabulator.addressable.drawComet(255,0,0,64,4,1);
      delay(10000);
      Encabulator.addressable.drawComet(0,255,0,64,4,1);
      delay(10000);
      Encabulator.addressable.drawComet(0,0,255,64,4,1);
      delay(10000);
  }
  
}

