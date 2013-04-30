#include <Encabulator.h>
#include <Wire.h>

int state = 0;
unsigned long kissStarted = 0;
unsigned long kissEnded = 0;
unsigned long scanStepStarted = 0;
int score = 0;

// delay (ms) between scanner steps (bars moving)
#define SCAN_INTERVAL_LOW 800
#define SCAN_INTERVAL_MED 400
#define SCAN_INTERVAL_HIGH 200
#define SCAN_BRIGHTNESS_LOW 100
#define SCAN_BRIGHTNESS_MED 175
#define SCAN_BRIGHTNESS_HIGH 255
unsigned int scanInterval = 0;

// buffer (ms) to allow brief breaks in contact
#define MAX_KISS_PAUSE 1000
// duration quantum (ms)
#define KISS_QUANTUM 2000

void setup() {               
  
    Encabulator.upUpDownDownLeftRightLeftRightBA();
    Encabulator.setVerbose(true);

    // clear the decks
    Encabulator.blackoutBars();
    Encabulator.addressable.blackout();

    // draw comet to show we're ready for business
    Encabulator.addressable.drawComet(0,255,255,64,4,1);
}

void loop() {
   
  // state 0: waiting for kiss to start
  // we leave this state as soon as we get input
  if (state == 0) {
      if (Encabulator.areTheyKissing()) {
          // kill the comet
          Encabulator.addressable.drawComet(0,0,0,64,4,1);

          // flash the bars red
          Encabulator.lightUpBars(8, 255, 0, 0);
          delay(100);
          Encabulator.blackoutBars();

          // start the scanner
          kissStarted = millis();
          kissEnded = 0;
          scanStepStarted = millis();
          scanInterval = SCAN_INTERVAL_LOW;
          Encabulator.setScannerColor(SCAN_BRIGHTNESS_LOW,0,0);
          Encabulator.startScanner();
          state = 1;
      }
  }

  // state 1: kissing in progress
  // run the scanner
  else if (state == 1) {
      if (Encabulator.areTheyKissing()) {
          // go to scoring after 8 quanta
          if ((millis() - kissStarted) > (8 * KISS_QUANTUM)) {
              kissEnded = millis();
              state = 3;
          }
          // speed up / brighten scanner with longer kisses
          else if ((millis() - kissStarted) > (4 * KISS_QUANTUM)) {
              scanInterval = SCAN_INTERVAL_HIGH;
              Encabulator.setScannerColor(SCAN_BRIGHTNESS_HIGH,0,0);
          }
          else if ((millis() - kissStarted) > (2 * KISS_QUANTUM)) {
              scanInterval = SCAN_INTERVAL_MED;
              Encabulator.setScannerColor(SCAN_BRIGHTNESS_MED,0,0);
          }

          // step the scanner if it's been scanInterval since the last one
          if ((millis() - scanStepStarted) > scanInterval) {
              scanStepStarted = millis();
              Encabulator.stepScanner();
          }
          
      }
      else {
          // kissing paused, set the timer and go to state 2
          kissEnded = millis();
          state = 2;
      }
  }

  // state 2: kissing paused
  // if kissing is interrupted for more than MAX_KISS_PAUSE, we go to scoring
  // otherwise we go back to state 1 and pick up where we left off
  else if (state == 2) {
      if (Encabulator.areTheyKissing()) {
          // oh good, they've started again.
          state = 1;
      }
      else {
          // has it been less than MAX_KISS_PAUSE? keep running the scanner
          if ((millis() - kissEnded) < MAX_KISS_PAUSE) {
              if ((millis() - scanStepStarted) > scanInterval) {
                  scanStepStarted = millis();
                  Encabulator.stepScanner();
              } 
          }
          // we're done here, move along.
          else {
              state = 3;
          }
      }
  }

  // state 3: scoring
  // we ignore input and spend some time displaying the score
  // then reset everything and go back to state 0
  else {
      Encabulator.blackoutBars();
      delay(1000);

      // score 1 bar, plus 1 for every quantum spent kissing
      score = ((kissEnded - kissStarted) / KISS_QUANTUM) + 1;

      // choose a score display randomly
      int scoreDisplay = random(1,7);
      if (scoreDisplay <= 1) {
          Encabulator.lightUpBars(score, 255, 255, 255); // white bars
      } else if (scoreDisplay == 2) {
          Encabulator.lightUpBars(score, 255, 255, 0); // yellow bars
      } else if (scoreDisplay == 3) {
          Encabulator.rainbowBars(score); // rainbow
      } else if (scoreDisplay == 4) {
          Encabulator.pulseBars(score, 255, 0, 0); // red pulse
      } else if (scoreDisplay == 5) {
          Encabulator.pulseBars(score, 0, 255, 0); // green pulse
      } else if (scoreDisplay >= 6) {
          Encabulator.pulseBars(score, 255, 255, 255); // white pulse
      }
      delay(5000);

      Encabulator.blackoutBars();
      delay(1000);

      // fire up the comet to tell the next couple to step up
      Encabulator.addressable.drawComet(0,255,255,64,4,1);
      state = 0;
  }

}
