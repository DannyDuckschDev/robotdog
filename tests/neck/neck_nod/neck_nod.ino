// Neck nod test (pitch, up/down) for Arduino / Elegoo Uno R3
// Wiring: servo signal (orange) -> breadboard row 9 -> Uno pin 3
//         servo red -> + strip, servo brown -> - strip
//         (strips fed by the separate USB power cable, Uno GND on the - strip)
//
// What it does: moves the neck slowly RANGE degrees to one side of CENTER,
// back to CENTER, RANGE degrees to the other side, back to CENTER.
// It does that CYCLES times after upload.
//
// Serial Monitor at 9600 baud, line ending "New Line":
//   press Enter         -> nod again
//   type e.g. 95, Enter -> move slowly to 95 and use it as the new center
//
// Which side is "up" depends on how the servo is mounted.

#include <Servo.h>

const int SERVO_PIN = 3;
const int RANGE = 45;        // degrees up and down from the center
const int STEP_DELAY = 50;   // ms per degree: bigger = slower
const int PAUSE = 400;       // ms to hold at each end
const int CYCLES = 3;        // nods per run

int center = 90;             // neck straight; change once you know the real value
int current;                 // where the servo is now

Servo neck;

// Move one degree at a time, so the neck never jumps.
void moveTo(int target) {
  while (current != target) {
    current += (target > current) ? 1 : -1;
    neck.write(current);
    delay(STEP_DELAY);
  }
}

void nod() {
  for (int i = 0; i < CYCLES; i++) {
    moveTo(center + RANGE);
    delay(PAUSE);
    moveTo(center);
    delay(PAUSE);
    moveTo(center - RANGE);
    delay(PAUSE);
    moveTo(center);
    delay(PAUSE);
  }
  Serial.println("Done. Enter = nod again, or type a new center angle.");
}

void setup() {
  Serial.begin(9600);
  current = center;
  neck.write(center);        // set position before attaching to avoid a jump
  neck.attach(SERVO_PIN);
  delay(1000);               // give the servo time to reach the center
  Serial.print("Neck nod test. Center ");
  Serial.print(center);
  Serial.print(", range +/-");
  Serial.println(RANGE);
  nod();
}

void loop() {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();

  if (line.length() == 0) {  // just Enter
    nod();
    return;
  }
  if (!isDigit(line[0])) return;   // ignore anything that is not a number

  // Keep the center far enough from 0 and 180 that center +/- RANGE stays valid.
  int newCenter = constrain(line.toInt(), RANGE, 180 - RANGE);
  moveTo(newCenter);
  center = newCenter;
  Serial.print("Center is now ");
  Serial.println(center);
}
