// Ultrasonic distance test (HC-SR04)
//
// Measures the distance to the nearest object about 10 times per second
// and prints it in centimetres to the Serial Monitor / Serial Plotter.
//
// Wiring:
//   VCC  (red)    -> Uno 5V
//   Trig (white)  -> A0   (used as a digital output)
//   Echo (purple) -> A1   (used as a digital input)
//   GND  (black)  -> breadboard - strip (shared with Uno GND)
//
// Serial Monitor: 9600 baud.

const int TRIG_PIN = A0;
const int ECHO_PIN = A1;

// The sensor's range ends at about 4 m. Sound needs about 23 ms for 4 m
// there and back, so we stop waiting for an echo after 25 ms (25000 µs).
const unsigned long ECHO_TIMEOUT_US = 25000;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);   // the Uno drives Trig
  pinMode(ECHO_PIN, INPUT);    // the Uno listens on Echo
  digitalWrite(TRIG_PIN, LOW);
  Serial.println("distance_cm");  // label for the Serial Plotter
}

// Sends one ultrasonic burst and returns the distance in cm,
// or -1 if no echo came back in time (nothing in range).
float measureDistanceCm() {
  // 1. Trigger pulse: LOW briefly for a clean start, then HIGH for 10 µs.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 2. Echo pulse: the sensor holds Echo HIGH for as long as the sound
  //    travelled. pulseIn() measures that time in microseconds (time of flight).
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) return -1;  // timeout: no echo

  // 3. Speed of sound: 0.0343 cm per µs. Divide by 2: there and back.
  return duration * 0.0343 / 2.0;
}

void loop() {
  float d = measureDistanceCm();
  Serial.println(d);
  delay(100);  // ~10 measurements per second; keeps echoes from overlapping
}
