// Bench firmware for Arduino / Elegoo Uno R3
// Lets the browser bench (bench/index.html) or the Serial Monitor drive pins
// with plain text commands. 115200 baud, one command per line.
//
//   PING              -> PONG
//   INFO              -> board and firmware version
//   HIGH 7 / LOW 7    -> set pin 7 to 5 V / 0 V (makes it an output)
//   PWM 9 128         -> PWM on pin 9, 0-255 (only pins 3 5 6 9 10 11)
//   IN 7 / PULLUP 7   -> make pin 7 an input (PULLUP: reads HIGH when open)
//   READ 7            -> VAL 7 0|1
//   AREAD A0          -> AVAL A0 0-1023
//   STOP              -> every output pin back to LOW
//
// Pins can be written as 7, D7 or A0. Pins 0 and 1 are the USB serial
// connection and are refused.

const char FW_VERSION[] = "bench 1.0";
const long BAUD = 115200;

// Pins this sketch has made outputs, so STOP can switch them all off.
bool isOutput[20];

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
  pinMode(pin, OUTPUT);
  isOutput[pin] = true;
}

void handle(char *cmd) {
  strtok(cmd, " ");  // cmd now holds just the command word
  char *arg2 = strtok(NULL, " ");
  char *arg3 = strtok(NULL, " ");
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
    makeOutput(pin);
    analogWrite(pin, constrain(atoi(arg3), 0, 255));
    ok(cmd, pin);
  } else if (!strcmp(cmd, "IN") || !strcmp(cmd, "PULLUP")) {
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
