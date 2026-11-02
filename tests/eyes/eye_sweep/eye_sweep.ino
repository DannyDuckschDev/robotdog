// Eye sweep test (left/right) for Arduino / Elegoo Uno R3
// Wiring: servo signal (orange) -> breadboard row 15 -> Uno pin 6
//         servo red -> + strip, servo brown -> - strip
//         (strips fed by the separate servo power cable, Uno GND on the - strip)
//
// What it does: looks slowly RANGE degrees to one side, back to the center,
// RANGE degrees to the other side, back to the center. CYCLES times.
//
// Serial Monitor at 9600 baud, line ending "New Line":
//   press Enter         -> sweep again
//   type e.g. 95, Enter -> move slowly to 95 and use it as the new center
//   type r, Enter       -> release: servo goes limp, so you can turn the gears by hand
//
// If the servo hums but the eyes do not move, the mechanism is too stiff:
// type r at once and fix the mechanics. A blocked servo gets hot.

#include <Servo.h>

const int SERVO_PIN = 6;
const int RANGE = 25;        // degrees left and right of the center; start small
const int STEP_DELAY = 30;   // ms per degree: bigger = slower
const int PAUSE = 200;       // ms to hold at each side
const int CYCLES = 3;        // sweeps per run

int center = 80;
int current;

Servo eye;

void moveTo(int target) {
  if (!eye.attached()) eye.attach(SERVO_PIN);
  while (current != target) {
    current += (target > current) ? 1 : -1;
    eye.write(current);
    delay(STEP_DELAY);
  }
}

void sweep() {
  for (int i = 0; i < CYCLES; i++) {
    moveTo(center - RANGE);
    delay(PAUSE);
    moveTo(center);
    delay(PAUSE);
    moveTo(center + RANGE);
    delay(PAUSE);
    moveTo(center);
    delay(PAUSE);
  }
  Serial.println("Done. Enter = sweep again, number = new center, r = release.");
}

void setup() {
  Serial.begin(9600);
  current = center;
  eye.write(center);         // set position before attaching to avoid a jump
  eye.attach(SERVO_PIN);
  delay(1000);
  Serial.print("Eye sweep test. Center ");
  Serial.print(center);
  Serial.print(", range +/-");
  Serial.println(RANGE);
  sweep();
}

void loop() {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();

  if (line.length() == 0) {
    sweep();
  } else if (line == "r" || line == "R") {
    eye.detach();
    Serial.println("Released. Enter or a number moves it again.");
  } else if (isDigit(line[0])) {
    int newCenter = constrain(line.toInt(), RANGE, 180 - RANGE);
    moveTo(newCenter);
    center = newCenter;
    Serial.print("Center is now ");
    Serial.println(center);
  }
}
