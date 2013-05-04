#include <Encabulator.h>
#include <Wire.h>

int scanCountdown = 0;
unsigned long lastStep = 0;
int scanMode = 0;

#define SCAN_INTERVAL 200

void setup() {               
  
  Encabulator.upUpDownDownLeftRightLeftRightBA();
  Encabulator.setVerbose(true);

  scanCountdown = 0;
  scanMode = 0;
}

void loop() {
   
  if (scanCountdown == 0) {
      scanCountdown = 100;
      scanMode++;
      if (scanMode == 3) {
        scanMode = 0;
      }
      lastStep = millis();
      // pick a random color and mode
      Encabulator.setScannerMode(scanMode);
      Encabulator.setScannerColorSimple(random(1,10));
      Encabulator.startScanner();
  }

  if ((millis() - lastStep) > SCAN_INTERVAL) {
      scanCountdown--;
      lastStep = millis();
      Encabulator.stepScanner();
  }
  
}
