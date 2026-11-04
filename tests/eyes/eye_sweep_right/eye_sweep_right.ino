// Right eye sweep test: Uno pin 5, breadboard row 19.
// Looks to one side, back to center, to the other side, back. Prints the
// angle at each side. Each side has its own range (RANGE_BELOW/ABOVE).
// MIRRORED makes it look the same way first as the left eye.
//
// Serial Monitor, 9600 baud, "New Line":
//   Enter = sweep again, a number = new center, r = release the servo.
// If the servo hums without moving, type r: a blocked servo gets hot.

#include <Servo.h>

const int SERVO_PIN = 5;
const bool MIRRORED = true;  // true = mounted mirror-image to the left eye
const int RANGE_BELOW = 45;  // degrees below the center: 80 - 45 = 35
const int RANGE_ABOVE = 35;  // degrees above the center: 80 + 35 = 115
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

// side = -1: the side the left eye looks to first, +1: the other side, 0: center.
void look(int side) {
  if (side == 0) { moveTo(center); return; }
  if (MIRRORED) side = -side;
  moveTo(side < 0 ? center - RANGE_BELOW : center + RANGE_ABOVE);
  Serial.print("At ");
  Serial.println(current);
}

void sweep() {
  for (int i = 0; i < CYCLES; i++) {
    look(-1);
    delay(PAUSE);
    look(0);
    delay(PAUSE);
    look(1);
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
  Serial.print(", from ");
  Serial.print(center - RANGE_BELOW);
  Serial.print(" to ");
  Serial.print(center + RANGE_ABOVE);
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
    int newCenter = constrain(line.toInt(), RANGE_BELOW, 180 - RANGE_ABOVE);
    moveTo(newCenter);
    center = newCenter;
    Serial.print("Center is now ");
    Serial.println(center);
  }
}
