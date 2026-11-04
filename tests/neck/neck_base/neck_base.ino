// Neck base test (yaw, turning left and right) for Arduino / Elegoo Uno R3
// Wiring: servo signal -> breadboard row 30 -> Uno pin 10
//         servo red -> + strip, servo brown -> - strip
//         (strips fed by the separate USB power cable, Uno GND on the - strip)
//
// Moves 1 degree every STEP_DELAY ms and never leaves SAFE_MIN..SAFE_MAX.
//
// Serial Monitor at 9600 baud, line ending "New Line":
//   type e.g. 100, Enter -> move slowly to 100
//   +, Enter             -> STEP degrees up
//   -, Enter             -> STEP degrees down

#include <Servo.h>

const int SERVO_PIN   = 10;
const int START_ANGLE = 90;
const int SAFE_MIN    = 45;
const int SAFE_MAX    = 135;
const int STEP        = 2;
const int STEP_DELAY  = 20;

Servo neck;
int currentAngle = START_ANGLE;

void moveSlowly(int target) {
  target = constrain(target, SAFE_MIN, SAFE_MAX);
  while (currentAngle != target) {
    currentAngle += (target > currentAngle) ? 1 : -1;
    neck.write(currentAngle);
    delay(STEP_DELAY);
  }
  Serial.print("Angle: ");
  Serial.println(currentAngle);
}

void setup() {
  Serial.begin(9600);
  neck.write(START_ANGLE);
  neck.attach(SERVO_PIN);
  Serial.println("Neck base test ready. Type an angle, + or -");
  Serial.print("Angle: ");
  Serial.println(currentAngle);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "+")      moveSlowly(currentAngle + STEP);
    else if (input == "-") moveSlowly(currentAngle - STEP);
    else if (input.length() > 0) moveSlowly(input.toInt());
  }
}
