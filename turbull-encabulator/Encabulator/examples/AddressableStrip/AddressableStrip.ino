#include <Encabulator.h>
#include <Wire.h>

int cometColor = 0;
unsigned long lastTick = 0;

#define CLOCK_TICK_LENGTH 4000

// test various methods for toggling the comet display on and off
// (in the first KissingBooth proto the comet would not turn back on) 
//
// if this works, you should see a red comet for 4 secs., then
// blackout for 4 secs., then green for 4, then black, then blue, then
// black again.  then the cycle repeats.

void setup() {               
  
  Encabulator.upUpDownDownLeftRightLeftRightBA();
  Encabulator.setVerbose(true);

  Encabulator.addressable.drawComet(255,0,0,64,4,1);
  cometColor = 0;
  lastTick = millis();
}

// comment/uncomment these to see which one works
void blackout() {
  Encabulator.addressable.blackout();
  //Encabulator.addressable.drawGradient(0,0,0,0,0,0,64);   
  //Encabulator.addressable.drawComet(0,0,0,8,8,1);
}

void loop() {

  if ((millis() - lastTick) > CLOCK_TICK_LENGTH) {

      if (cometColor == 1) {
          Encabulator.addressable.drawComet(0,255,0,64,4,1);
      }
      else if (cometColor == 3) {
          Encabulator.addressable.drawComet(0,0,255,64,4,1);
      }
      else if (cometColor == 5) {
          Encabulator.addressable.drawComet(255,0,0,64,4,1);
      }
      else {
          blackout();
      }

      cometColor++;
      if (cometColor > 5) {
        cometColor = 0;
      }

      lastTick = millis();
  }

}

