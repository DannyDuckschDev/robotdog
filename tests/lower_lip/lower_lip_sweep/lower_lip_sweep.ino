// Lower lip test: Uno pin 8, breadboard row 25.
// Starts closed, opens slowly to OPEN, closes again back to CLOSED.
// Does that CYCLES times after upload. Prints the angle at each end.
// CLOSED is bigger than OPEN here because of how the servo is mounted.
//
// Serial Monitor, 9600 baud, "New Line":
//   Enter     = open and close again
//   a number  = move slowly to that angle (to find the exact closed/open angles)
//   r         = release the servo
// If the servo hums without moving, type r: a blocked servo gets hot.

#include <Servo.h>

const int SERVO_PIN = 8;
const int CLOSED = 165;      // neutral, mouth closed
const int OPEN = 80;         // fully open
const int STEP_DELAY = 30;   // ms per degree: bigger = slower
const int PAUSE = 300;       // ms to hold at each end
const int CYCLES = 3;        // open/close movements per run

int current = CLOSED;

Servo lip;

void moveTo(int target) {
  if (!lip.attached()) lip.attach(SERVO_PIN);
  while (current != target) {
    current += (target > current) ? 1 : -1;
    lip.write(current);
    delay(STEP_DELAY);
  }
  Serial.print("At ");
  Serial.println(current);
}

void cycle() {
  moveTo(CLOSED);            // always start from closed
  delay(PAUSE);
  for (int i = 0; i < CYCLES; i++) {
    moveTo(OPEN);
    delay(PAUSE);
    moveTo(CLOSED);
    delay(PAUSE);
  }
  Serial.println("Done. Enter = again, number = move there, r = release.");
}

void setup() {
  Serial.begin(9600);
  lip.write(CLOSED);         // set position before attaching to avoid a jump
  lip.attach(SERVO_PIN);
  delay(1000);
  Serial.print("Lower lip test. Closed ");
  Serial.print(CLOSED);
  Serial.print(", open ");
  Serial.println(OPEN);
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
    moveTo(constrain(line.toInt(), 0, 180));
  }
}
