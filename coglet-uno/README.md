# Coglet Uno

Arduino Uno hardware notes and sketches for the Coglet robot dog.

## Pin map

Record the assigned Uno pin for each servo signal before running a test. Pins
and connections are not assigned yet.

| Device | Signal | Uno pin |
| --- | --- | --- |
| Jaw servo | Signal | TBD |

## Wiring

- Connect each servo's signal wire to its assigned Uno pin.
- Connect servo power and ground according to the servo's rated voltage and
  current; do not power servos from the Uno's 5 V pin.
- Use a suitably rated external servo supply and connect its ground to Uno
  ground.
- Verify the pin map and supply wiring before connecting or powering a servo.

## Power notes

Servo current demand can exceed what the Uno or its USB connection can supply.
Use a separate supply sized for the servos, observe its polarity and voltage
rating, and switch power off before changing wiring. Do not run a servo test
until its pin and calibration values have been recorded.

## Project layout

- `calibration.md` records each servo's minimum, maximum, and neutral angles.
- `tests/` contains an individual sketch for each servo; add a folder and test
  sketch for every servo as it is identified.
- `coglet/coglet.ino` is reserved for the main sketch, which will be developed
  after the individual servo tests.
