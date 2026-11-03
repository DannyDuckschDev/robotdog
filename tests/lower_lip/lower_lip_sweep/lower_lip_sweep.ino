// Lower lip sweep test: Uno pin 8, breadboard row 25.
// NOT WORKING YET: the lip does not move. It is fixed to the head with a
// screw and has more resistance than the other parts; the servo may be too weak.
// Moves RANGE_ABOVE degrees up, back to center, RANGE_BELOW degrees down,
// back to center. Prints the angle at each end. Which way is "up" depends
// on how the servo is mounted.
//
// Serial Monitor, 9600 baud, "New Line":
//   Enter = move again, a number = new center, r = release the servo.
// If the servo hums without moving, type r: a blocked servo gets hot.

#include <Servo.h>

const int SERVO_PIN = 8;
const int RANGE_ABOVE = 20;  // degrees above the center: 90 + 20 = 110
const int RANGE_BELOW = 20;  // degrees below the center: 90 - 20 = 70
const int STEP_DELAY = 30;   // ms per degree: bigger = slower
const int PAUSE = 300;       // ms to hold at each end
const int CYCLES = 3;        // up/down movements per run

int center = 90;
int current;

Servo lip;

void moveTo(int target) {
  if (!lip.attached()) lip.attach(SERVO_PIN);
  while (current != target) {
    current += (target > current) ? 1 : -1;
    lip.write(current);
    delay(STEP_DELAY);
  }
}

void moveAndReport(int target) {
  moveTo(target);
  Serial.print("At ");
  Serial.println(current);
}

void cycle() {
  for (int i = 0; i < CYCLES; i++) {
    moveAndReport(center + RANGE_ABOVE);
    delay(PAUSE);
    moveTo(center);
    delay(PAUSE);
    moveAndReport(center - RANGE_BELOW);
    delay(PAUSE);
    moveTo(center);
    delay(PAUSE);
  }
  Serial.println("Done. Enter = move again, number = new center, r = release.");
}

void setup() {
  Serial.begin(9600);
  current = center;
  lip.write(center);       // set position before attaching to avoid a jump
  lip.attach(SERVO_PIN);
  delay(1000);
  Serial.print("Lower lip sweep test. Center ");
  Serial.print(center);
  Serial.print(", from ");
  Serial.print(center - RANGE_BELOW);
  Serial.print(" to ");
  Serial.println(center + RANGE_ABOVE);
  cycle();
}

void loop() {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();

  if (line.length() == 0) {
    cycle();
  } else if (line == "r" || line == "R") {
    lip.detach();
    Serial.println("Released. Enter or a number moves it again.");
  } else if (isDigit(line[0])) {
    int newCenter = constrain(line.toInt(), RANGE_BELOW, 180 - RANGE_ABOVE);
    moveTo(newCenter);
    center = newCenter;
    Serial.print("Center is now ");
    Serial.println(center);
  }
}
