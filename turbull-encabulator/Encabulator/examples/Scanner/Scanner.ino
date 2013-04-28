#include <Encabulator.h>
#include <Wire.h>

int i = 0;
int j = 0;

void setup() {               
  
  Encabulator.upUpDownDownLeftRightLeftRightBA();
  Encabulator.setVerbose(true);

}

void loop() {
   
    Encabulator.startRedScanner();

    // cycle through four times, getting faster each time
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            Encabulator.stepScanner();
            delay((5-i)*100);
        }
    }
  
}
