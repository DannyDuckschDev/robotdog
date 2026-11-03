// Right eye sweep test (left/right) for Arduino / Elegoo Uno R3
// Wiring: servo signal (orange) -> breadboard row (own row) -> Uno pin 5
//         servo red -> + strip, servo brown -> - strip
//         (strips fed by the separate servo power cable, Uno GND on the - strip)
//
// What it does: looks slowly RANGE degrees to one side, back to the center,
// RANGE degrees to the other side, back to the center. CYCLES times.
// The first look goes the same way as the left eye's first look
// (tests/eyes/eye_sweep), as long as MIRRORED is set correctly.
//
// Serial Monitor at 9600 baud, line ending "New Line":
//   press Enter         -> sweep again
//   type e.g. 95, Enter -> move slowly to angle 95 and use it as the new center
//   type r, Enter       -> release: servo goes limp, so you can turn the gears by hand
//
// Finding the settings:
// 1. center: type angles until the eye looks straight ahead. Write that
//    number into `center` below and into docs/calibration.md.
// 2. MIRRORED: run the left eye sketch, watch which way it looks first,
//    then run this one. If the right eye looks the other way first, flip
//    MIRRORED. With the servos mounted mirror-image, it is usually true.
//
// If the servo hums but the eye does not move, the mechanism is too stiff:
// type r at once and fix the mechanics. A blocked servo gets hot.

#include <Servo.h>

const int SERVO_PIN = 5;
const bool MIRRORED = true;  // true = a higher angle turns this eye the other way than the left eye
const int RANGE = 25;        // degrees left and right of the center; start small
const int STEP_DELAY = 30;   // ms per degree: bigger = slower
const int PAUSE = 200;       // ms to hold at each side
const int CYCLES = 3;        // sweeps per run

int center = 80;             // starting guess (left eye's value); replace with the measured one
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

// Look `offset` degrees from the center, in the left eye's direction.
// Negative = the side the left eye looks to first.
void look(int offset) {
  moveTo(center + (MIRRORED ? -offset : offset));
}

void sweep() {
  for (int i = 0; i < CYCLES; i++) {
    look(-RANGE);
    delay(PAUSE);
    look(0);
    delay(PAUSE);
    look(RANGE);
    delay(PAUSE);
    look(0);
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
  Serial.print("Right eye sweep test. Center ");
  Serial.print(center);
  Serial.print(", range +/-");
  Serial.print(RANGE);
  Serial.println(MIRRORED ? ", mirrored" : ", not mirrored");
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
