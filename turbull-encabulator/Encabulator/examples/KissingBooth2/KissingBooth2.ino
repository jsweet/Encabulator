#include <Encabulator.h>
#include <Wire.h>

int state = 0;
unsigned long kissStarted = 0;
unsigned long kissEnded = 0;
unsigned long scanStepStarted = 0;
unsigned int score = 0;
unsigned int i = 0;
unsigned int waitBar = 0;

// delay (ms) between scanner steps (bars moving)
#define SCAN_INTERVAL_LOW 800
#define SCAN_INTERVAL_MED 400
#define SCAN_INTERVAL_HIGH 200
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

    // pick a random color
    Encabulator.setScannerColorSimple(random(9)+1);
    waitBar = random(1,9);
    Encabulator.jumpBar(waitBar);
    kissEnded = millis();
}

void loop() {
   
  // state 0: waiting for kiss to start
  // we leave this state as soon as we get input
  if (state == 0) {
      if (Encabulator.areTheyKissing()) {
          // flash the bars
          for (i = 1; i < 9; i++) {
              Encabulator.jumpBar(i);
          }
          delay(100);
          Encabulator.blackoutBars();

          // start the scanner
          kissStarted = millis();
          kissEnded = 0;
          scanStepStarted = millis();
          scanInterval = SCAN_INTERVAL_LOW;
          Encabulator.setScannerColorSimple(random(9)+1);
          Encabulator.setScannerMode(random(3));
          Encabulator.startScanner();
          state = 1;
      }
      else if ((millis() - kissEnded) > (SCAN_INTERVAL_LOW * 2)) {
        // cycle the waitbar
        Encabulator.jumpBarRGB(waitBar,0,0,0);
        Encabulator.setScannerColorSimple(random(9)+1);
        waitBar = random(1,9);
        Encabulator.jumpBar(waitBar);
        kissEnded = millis();
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
          }
          else if ((millis() - kissStarted) > (2 * KISS_QUANTUM)) {
              scanInterval = SCAN_INTERVAL_MED;
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

      // score 1 for every quantum spent kissing, plus 0, 1 or 2
      score = ((kissEnded - kissStarted) / KISS_QUANTUM) + random(3);
      if (score < 1) { score = 1; }
      if (score > 8) { score = 8; }

      // choose a score display randomly
      int scoreDisplay = random(7);
      if (scoreDisplay < 3) {
          Encabulator.lightUpBars(score, 255, 255, 255); // white bars
      } else if (scoreDisplay < 6) {
          Encabulator.lightUpBars(score, 255, 255, 0); // yellow bars
      } else {
          Encabulator.rainbowBars(score); // rainbow
      }
      delay(5000);

      Encabulator.blackoutBars();
      delay(1000);

      // fire up the waitbar to tell the next couple to step up
      Encabulator.setScannerColorSimple(random(9)+1);
      waitBar = random(1,9);
      Encabulator.jumpBar(waitBar);
      state = 0;
  }

}
