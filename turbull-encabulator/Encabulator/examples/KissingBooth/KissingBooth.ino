#include <Encabulator.h>
#include <Wire.h>

int state = 0;
unsigned long kissStarted = 0;
unsigned long kissEnded = 0;
unsigned long scanStepStarted = 0;
int score = 0;

// delay (ms) between scanner steps (bars moving)
// TBD: speed this up with longer kiss duration
#define SCAN_STEP_INTERVAL 200

// buffer (ms) to allow brief breaks in contact
#define MAX_KISS_PAUSE 1000

void setup() {               
  
    Encabulator.upUpDownDownLeftRightLeftRightBA();
    Encabulator.setVerbose(true);

    // clear the decks
    Encabulator.blackoutBars();
    Encabulator.addressable.drawGradient(0,0,0,0,0,0,64);

    // draw comet to show we're ready for business
    Encabulator.addressable.drawComet(0,255,255,64,4,1);
}

void loop() {
   
  // state 0: waiting for kiss to start
  // we leave this state as soon as we get input
  if (state == 0) {
      if (Encabulator.areTheyKissing()) {
          // kill the comet
          Encabulator.addressable.drawGradient(0,0,0,0,0,0,64);

          // flash the bars red
          Encabulator.lightUpBars(8, 255, 0, 0);
          delay(100);
          Encabulator.blackoutBars();

          // start the scanner
          kissStarted = millis();
          kissEnded = 0;
          scanStepStarted = millis();
          Encabulator.startRedScanner();
          state = 1;
      }
  }

  // state 1: kissing in progress
  // run the scanner
  else if (state == 1) {
      if (Encabulator.areTheyKissing()) {
          // step the scanner if it's been SCAN_STEP_INTERVAL since the last one
          // TBD: speed up / brighten scanner with longer kisses
          if ((millis() - scanStepStarted) > SCAN_STEP_INTERVAL) {
              scanStepStarted = millis();
              Encabulator.stepScanner();
          }
          
          // TBD: go to state 3 after max kiss duration
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
              if ((millis() - scanStepStarted) > SCAN_STEP_INTERVAL) {
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
      // TBD: something more interesting here
      Encabulator.blackoutBars();
      delay(1000);
      // score 1 bar, plus 1 for every 2 seconds spent kissing (max 8)
      score = ((kissEnded - kissStarted) / 2000) + 1;
      // just white for now
      Encabulator.lightUpBars(score, 255, 255, 255);
      delay(5000);
      Encabulator.blackoutBars();
      delay(1000);
      // fire up the comet to tell the next couple to step up
      Encabulator.addressable.drawComet(0,255,255,64,4,1);
      state = 0;
  }

}
