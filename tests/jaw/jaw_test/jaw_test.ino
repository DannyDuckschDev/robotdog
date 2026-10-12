// Jaw servo test for Arduino / Elegoo Uno R3
// Wiring: servo signal (orange/yellow) -> pin 9, red -> 5V, brown/black -> GND
// Use: open Serial Monitor at 9600 baud, type an angle (0-180), press Enter.

#include <Servo.h>

const int SERVO_PIN = 9;
const int START_ANGLE = 90;   // servo goes here on power-up

Servo jaw;

void setup() {
  Serial.begin(9600);
  jaw.write(START_ANGLE);     // set position before attaching to avoid a jump
  jaw.attach(SERVO_PIN);
  Serial.println("Jaw servo test. Type an angle 0-180 and press Enter.");
}

void loop() {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();

  // Ignore empty lines and anything that is not a number,
  // so stray input never sends the servo to 0.
  if (line.length() == 0 || !isDigit(line[0])) return;

  int angle = constrain(line.toInt(), 0, 180);
  jaw.write(angle);

  Serial.print("Moved to ");
  Serial.println(angle);
}
