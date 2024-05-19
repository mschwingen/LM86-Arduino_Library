/******************************************************************************
Test / sample program for LM86 library (on esp32)
(C) 2024 Michael Schwingen <michael@schwingen.org>

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
******************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "LM86.h"

# define LED_PIN 2
# define SCL_PIN 4
# define SDA_PIN 16

LM86 lm86;

void setup() {
  digitalWrite(LED_PIN, 1);

  Serial.begin(115200);
  Serial.println("\nLM86-Test starting...");

  Wire.begin(SDA_PIN, SCL_PIN);
  // Init LM86 in oneshot mode, since this saves power and continuous mode
  // makes it difficult to get consistent 16-bit temperature values.
  if (!lm86.begin(Wire, LM86::ConvRate::CR_ONESHOT)) {
    Serial.println("LM86 init failed.");
  }
}

void loop() {
  lm86.oneshot(); // trigger one conversion and wait for result
  Serial.printf("\nTemp Local: %f", lm86.getLocalTemperature());
  Serial.printf("\nTemp Remote: %f", lm86.getRemoteTemperature());

  delay(2000);
}


// Local variables:
// compile-command: "cd ~/workspace/arduino/esp32/lm86/; platformio --no-ansi run -e lm86test"
// End:
