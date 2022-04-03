// Arduino Brain Library - Brain Serial Test

// Description: Grabs brain data from the serial RX pin and sends CSV out over the TX pin (Half duplex.)
// More info: https://github.com/kitschpatrol/Arduino-Brain-Library
// Author: Eric Mika, 2010 revised in 2014
#include <Brain.h>

// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial);

uint32_t ab = 0;
unsigned long int abc=0;
unsigned long int prev_abc=0;

void setup() {
  // Start the hardware serial.
  Serial.begin(57600);
}

void loop() {
  // Expect packets about once per second.
  // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
  // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"
  if (brain.update())
  {
    //Serial.println(brain.readCSV());
    if (brain.readSignalQuality() == 0)
    {
      //Serial.print("CSV: ");Serial.println(brain.readCSV());
      abc = brain.readLowAlpha();
      if (abc != 0 && abc!= prev_abc)
      {
        prev_abc = abc;
        Serial.print("CSV: ");Serial.println(brain.readCSV());
        //Serial.print("readAttention:   "); Serial.println(brain.readAttention());
        //Serial.print("readMeditation:  "); Serial.println(brain.readMeditation());
        //Serial.print("readLowAlpha:    "); Serial.println(brain.readLowAlpha());
//        Serial.print("readHighAlpha:   "); Serial.println(brain.readHighAlpha());
//        Serial.print("readLowBeta:     "); Serial.println(brain.readLowBeta());
//        Serial.print("readHighBeta:    "); Serial.println(brain.readHighBeta());
//        Serial.print("readLowGamma:    "); Serial.println(brain.readLowGamma());
//        Serial.print("readMidGamma:    "); Serial.println(brain.readMidGamma());
//        Serial.print("readDelta:       "); Serial.println(brain.readDelta());
//        Serial.print("readTheta:       "); Serial.println(brain.readTheta());
      }

    }
  }
}
