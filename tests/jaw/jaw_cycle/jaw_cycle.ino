// Jaw open/close cycle for Arduino / Elegoo Uno R3
// Wiring: servo signal (orange/yellow) -> pin 9, red -> 5V, brown/black -> GND
// Use: upload, and the jaw opens and closes CYCLES times.
//      Open Serial Monitor at 9600 baud and press Enter to run it again.

#include <Servo.h>

const int SERVO_PIN = 9;
const int CLOSED_ANGLE = 0;    // mouth fully closed
const int OPEN_ANGLE = 80;     // mouth fully open
const int CYCLES = 5;          // how many open/close movements per run
const int STEP_DELAY = 10;     // ms per degree: bigger = slower movement
const int PAUSE = 500;         // ms to hold when fully open or closed

Servo jaw;

void setup() {
  Serial.begin(9600);
  jaw.write(CLOSED_ANGLE);     // set position before attaching to avoid a jump
  jaw.attach(SERVO_PIN);
  delay(1000);                 // give the servo time to reach the start
  runCycles();
}

void loop() {
  if (!Serial.available()) return;
  while (Serial.available()) Serial.read();   // throw away what was typed
  runCycles();
}

void runCycles() {
  for (int i = 1; i <= CYCLES; i++) {
    Serial.print("Cycle ");
    Serial.print(i);
    Serial.print(" of ");
    Serial.println(CYCLES);
    moveTo(OPEN_ANGLE);
    delay(PAUSE);
    moveTo(CLOSED_ANGLE);
    delay(PAUSE);
  }
  Serial.println("Done. Press Enter to run again.");
}

// Moves one degree at a time, so the speed is controlled by STEP_DELAY
// instead of the servo jumping at full speed.
void moveTo(int target) {
  int angle = jaw.read();
  while (angle != target) {
    angle += (target > angle) ? 1 : -1;
    jaw.write(angle);
    delay(STEP_DELAY);
  }
}
