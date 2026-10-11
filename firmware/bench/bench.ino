// Bench firmware for Arduino / Elegoo Uno R3
// Lets the browser bench (bench/index.html) or the Serial Monitor drive pins
// with plain text commands. 115200 baud, one command per line.
//
//   PING              -> PONG
//   INFO              -> board and firmware version
//   HIGH 7 / LOW 7    -> set pin 7 to 5 V / 0 V (makes it an output)
//   PWM 3 128         -> PWM on pin 3, 0-255 (only pins 3 5 6 9 10 11)
//   IN 7 / PULLUP 7   -> make pin 7 an input (PULLUP: reads HIGH when open)
//   READ 7            -> VAL 7 0|1
//   AREAD A0          -> AVAL A0 0-1023
//   SERVO 9 80        -> servo on pin 9 jumps to 80 degrees
//   SERVO 9 80 10     -> same, but moves 1 degree every 10 ms (slower)
//                        When the servo arrives: AT 9 80
//   DETACH 9          -> switch the servo signal off (servo goes limp)
//   STOP              -> every output pin LOW, every servo holds where it is
//
// Pins can be written as 7, D7 or A0. Pins 0 and 1 are the USB serial
// connection and are refused.
//
// While any servo is attached, pins 9 and 10 cannot do PWM: the Servo
// library uses the timer that drives them.

#include <Servo.h>

const char FW_VERSION[] = "bench 1.1";
const long BAUD = 115200;
const byte SERVO_SLOTS = 8;

// Pins this sketch has made outputs, so STOP can switch them all off.
bool isOutput[20];

struct ServoSlot {
  byte pin;              // 0 = slot unused
  Servo servo;
  int current;
  int target;
  int stepMs;            // ms per degree, 0 = jump
  unsigned long lastStep;
};
ServoSlot slots[SERVO_SLOTS];

char line[32];
byte lineLen = 0;

void setup() {
  Serial.begin(BAUD);
  Serial.print("READY ");
  Serial.println(FW_VERSION);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      line[lineLen] = '\0';
      if (lineLen > 0) handle(line);
      lineLen = 0;
    } else if (lineLen < sizeof(line) - 1) {
      line[lineLen++] = toupper(c);
    }
  }
  updateServos();
}

// Moves every servo one degree closer to its target when its step time is up.
void updateServos() {
  unsigned long now = millis();
  for (byte i = 0; i < SERVO_SLOTS; i++) {
    ServoSlot &s = slots[i];
    if (s.pin == 0 || s.current == s.target) continue;
    if (now - s.lastStep < (unsigned long)s.stepMs) continue;
    s.current += (s.target > s.current) ? 1 : -1;
    s.servo.write(s.current);
    s.lastStep = now;
    if (s.current == s.target) reportAt(s);
  }
}

void reportAt(ServoSlot &s) {
  Serial.print("AT ");
  Serial.print(s.pin);
  Serial.print(' ');
  Serial.println(s.current);
}

ServoSlot *findServo(int pin) {
  for (byte i = 0; i < SERVO_SLOTS; i++) {
    if (slots[i].pin == pin) return &slots[i];
  }
  return NULL;
}

bool anyServo() {
  for (byte i = 0; i < SERVO_SLOTS; i++) {
    if (slots[i].pin != 0) return true;
  }
  return false;
}

void detachServo(int pin) {
  ServoSlot *s = findServo(pin);
  if (s == NULL) return;
  s->servo.detach();
  s->pin = 0;
}

// Returns the pin number for "7", "D7" or "A0", or -1 if invalid.
int parsePin(const char *s) {
  if (s == NULL) return -1;
  int base = 0;
  if (*s == 'A') { base = A0; s++; }
  else if (*s == 'D') { s++; }
  if (!isDigit(*s)) return -1;
  int n = base + atoi(s);
  if (base == A0 && n > A5) return -1;
  if (n < 2 || n > A5) return -1;
  return n;
}

bool isPwmPin(int pin) {
  return pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11;
}

void err(const char *msg) {
  Serial.print("ERR ");
  Serial.println(msg);
}

void ok(const char *cmd, int pin) {
  Serial.print("OK ");
  Serial.print(cmd);
  Serial.print(' ');
  Serial.println(pin);
}

void makeOutput(int pin) {
  detachServo(pin);
  pinMode(pin, OUTPUT);
  isOutput[pin] = true;
}

void moveServo(int pin, int angle, int stepMs) {
  ServoSlot *s = findServo(pin);
  if (s == NULL) {
    // New servo: it has no known position, so it jumps to the first angle.
    s = findServo(0);
    if (s == NULL) { err("too many servos (max 8)"); return; }
    isOutput[pin] = false;
    s->pin = pin;
    s->servo.write(angle);
    s->servo.attach(pin);
    s->current = angle;
    s->target = angle;
    ok("SERVO", pin);
    reportAt(*s);
    return;
  }
  s->target = angle;
  s->stepMs = stepMs;
  ok("SERVO", pin);
  if (stepMs == 0 || s->current == angle) {
    s->current = angle;
    s->servo.write(angle);
    reportAt(*s);
  }
}

void handle(char *cmd) {
  strtok(cmd, " ");  // cmd now holds just the command word
  char *arg2 = strtok(NULL, " ");
  char *arg3 = strtok(NULL, " ");
  char *arg4 = strtok(NULL, " ");
  int pin = parsePin(arg2);

  if (!strcmp(cmd, "PING")) {
    Serial.println("PONG");
  } else if (!strcmp(cmd, "INFO")) {
    Serial.print("INFO board=uno fw=");
    Serial.println(FW_VERSION);
  } else if (!strcmp(cmd, "STOP")) {
    for (int p = 2; p <= A5; p++) {
      if (isOutput[p]) digitalWrite(p, LOW);
    }
    for (byte i = 0; i < SERVO_SLOTS; i++) {
      if (slots[i].pin != 0) slots[i].target = slots[i].current;
    }
    Serial.println("OK STOP");
  } else if (pin < 0) {
    err(arg2 == NULL ? "missing pin" : "bad pin (use 2-13 or A0-A5)");
  } else if (!strcmp(cmd, "HIGH") || !strcmp(cmd, "LOW")) {
    makeOutput(pin);
    digitalWrite(pin, cmd[0] == 'H' ? HIGH : LOW);
    ok(cmd, pin);
  } else if (!strcmp(cmd, "PWM")) {
    if (!isPwmPin(pin)) { err("no PWM on this pin (use 3 5 6 9 10 11)"); return; }
    if (arg3 == NULL) { err("missing value 0-255"); return; }
    detachServo(pin);
    if ((pin == 9 || pin == 10) && anyServo()) { err("no PWM on 9 and 10 while servos are attached"); return; }
    makeOutput(pin);
    analogWrite(pin, constrain(atoi(arg3), 0, 255));
    ok(cmd, pin);
  } else if (!strcmp(cmd, "SERVO")) {
    if (arg3 == NULL) { err("missing angle 0-180"); return; }
    int stepMs = arg4 == NULL ? 0 : constrain(atoi(arg4), 0, 200);
    moveServo(pin, constrain(atoi(arg3), 0, 180), stepMs);
  } else if (!strcmp(cmd, "DETACH")) {
    detachServo(pin);
    ok(cmd, pin);
  } else if (!strcmp(cmd, "IN") || !strcmp(cmd, "PULLUP")) {
    detachServo(pin);
    pinMode(pin, cmd[0] == 'P' ? INPUT_PULLUP : INPUT);
    isOutput[pin] = false;
    ok(cmd, pin);
  } else if (!strcmp(cmd, "READ")) {
    Serial.print("VAL ");
    Serial.print(arg2);
    Serial.print(' ');
    Serial.println(digitalRead(pin));
  } else if (!strcmp(cmd, "AREAD")) {
    if (pin < A0) { err("AREAD needs A0-A5"); return; }
    Serial.print("AVAL ");
    Serial.print(arg2);
    Serial.print(' ');
    Serial.println(analogRead(pin));
  } else {
    err("unknown command");
  }
}
