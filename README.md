# Robotdog

An interactive robot dog artwork, built for a Digital Fabrication module.

The piece continues an artwork about fear of dogs, and how that fear turned into wanting to pet one every day. Learning to read canine body language was part of that change, so the robot's job is to show it: ears up or pinned back, a tense or relaxed body, a wagging tail.

The dog is meant to look cute. That is a deliberate choice against the hard, aggressive look of most robot dogs.

## Status

Early prototyping. Working on the head first, one servo at a time, on a borrowed Arduino kit.

- [x] Cardboard prototype of the head
- [x] Coglet head assembled digitally in Fusion 360
- [x] Jaw servo test sketch written (`tests/jaw_test`)
- [ ] Jaw servo tested and calibrated on hardware
- [ ] Remaining head servos tested and calibrated
- [ ] Head printed and assembled
- [ ] Sesame body built and walking with its own firmware
- [ ] Tail mechanism designed
- [ ] Head mounted on body, electronics consolidated

## What it is built from

The robot combines three open-source references. Reshaping them into a dog, the ear and tail work, and the integration are original.

| Part | Source | Used for | Licence |
|---|---|---|---|
| Head | [Coglet](https://github.com/will-cogley/Coglet) by Will Cogley | Servo-driven mouth, eyes, eyelid, ears, neck | CC BY-NC 4.0 (non-commercial) |
| Body and legs | [Sesame](https://github.com/dorianborian/sesame-robot) by Dorian Todd | Chassis, legs and walking gait, used as is. Its OLED face is left out; the Coglet head replaces it | Apache 2.0 |
| Tail and ear behaviour | [OpenTails / CRUMPET](https://github.com/OpenTails) | Movement patterns only (wag and tremble styles). It is a control app for commercial tails, not an open mechanism, so the tail mechanism is designed from scratch | GPL-3.0 (app) |

Licences are as noted during planning. Check each upstream `LICENSE` file before reusing anything from here.

### Planned changes to the head

The cardboard prototype goes further than Coglet:

- Ears with three poses: lifted, turned sideways, pinned flat back. Coglet's ears only stand up and fold back.
- Separate upper and lower lip.
- A tongue with three positions.

Coglet's head is printed tight, so extra servos mean enlarging the head.

## Electronics

### Current setup: Elegoo Uno R3

Coglet runs on its own CogNog board (ESP32 plus RP2040). This build currently uses an **Elegoo Uno R3** instead, to test and calibrate servos one at a time.

The Uno cannot run the Coglet or Sesame firmware as is, so every sketch in this repo is written for the Uno. The final robot will most likely need an ESP32 for Sesame's gait firmware; see open questions.

### Wiring one servo

The step-by-step version with photos, including the breadboard, is [`docs/setup-guide.md`](docs/setup-guide.md).

| Servo wire | Uno pin |
|---|---|
| Brown or black (ground) | `GND` |
| Red (power) | `5V` |
| Orange or yellow (signal) | a digital pin, see the pin map |

The servo plug is female. Use three male-to-male jumper wires between the plug and the Uno headers. Swapping red and brown can destroy the servo, so check the colours before plugging in USB.

### Power

One micro servo runs from the Uno's `5V` pin over USB. If the Uno resets or the servo twitches, it is short on power.

More than two or three servos need a separate 5 V supply. Connect its ground to the Uno's `GND`, or the signal has no reference and the servos behave randomly.

### Pin map

Coglet's nine head servos, with the Uno pin each one gets. The live version, with breadboard rows and angles, is [`bench/wiring.json`](bench/wiring.json). Pins 0 and 1 stay free because the USB serial connection uses them.

| Servo | Uno pin | Status |
|---|---|---|
| Mouth (jaw) | 9 | Sketch written |
| Yaw (neck) | 2 | Planned |
| Roll (neck) | 3 | Planned |
| Pitch (neck) | 4 | Planned |
| Left eye | 5 | Planned |
| Right eye | 6 | Planned |
| Eyelid | 7 | Planned |
| Left ear | 8 | Planned |
| Right ear | 10 | Planned |

Measured angles for each servo go in [`docs/calibration.md`](docs/calibration.md).

## Repo layout

```
robotdog/
├── README.md
├── bench.sh                  start the browser bench
├── flash.sh                  upload the bench firmware to the Uno
├── bench/
│   ├── index.html            browser bench: wiring drawing, servo and pin controls, terminal
│   ├── server.py             serves the bench and saves wiring.json
│   └── wiring.json           which part is on which pin, row and wire colour
├── firmware/
│   └── bench/
│       └── bench.ino         text-command firmware the bench talks to
├── docs/
│   ├── calibration.md        closed / open / neutral angle per servo
│   ├── setup-guide.md        how the jaw was wired, with photos, and how to add the next servo
│   └── images/               screenshots and setup photos
└── tests/
    ├── jaw_test/
    │   └── jaw_test.ino      move the jaw servo to a typed angle
    └── jaw_cycle/
        └── jaw_cycle.ino     open and close the jaw a few times
```

## Working with the sketches

In Arduino, one **sketch** is one program: a folder containing a `.ino` file of the same name. That is why `jaw_test.ino` sits in a folder called `jaw_test`.

The Uno holds one program at a time. Uploading a sketch replaces whatever was on the board before. So there is no single "project" in the Arduino IDE; this repo is the project, and each test is its own sketch.

To run a test:

1. In the Arduino IDE, choose File → Open and pick the `.ino` file, for example `tests/jaw_test/jaw_test.ino`. Each sketch opens in its own window.
2. Select the board (Tools → Board → Arduino Uno) and the port (Tools → Port, the entry with "usbmodem" or "usbserial" in its name).
3. Click Upload (the arrow button).
4. Open the Serial Monitor (Tools → Serial Monitor) and set it to 9600 baud.

To add a test, create a new folder under `tests/` and put a `.ino` file with the same name inside it.

If the compiler reports that `Servo.h` is missing, install the "Servo" library via Tools → Manage Libraries.

### Tests

| Sketch | What it does |
|---|---|
| `tests/jaw_test` | Type an angle from 0 to 180 in the Serial Monitor; the jaw servo moves there. Used to find the closed and open angles. |
| `tests/jaw_cycle` | Opens and closes the jaw between two set angles a few times, at a set speed. Press Enter in the Serial Monitor to run it again. |

## Browser bench

A page for driving the Uno's pins live from Chrome, with a clickable pinout and a serial terminal. Meant for testing the motor driver wiring before writing a real sketch.

1. Flash the bench firmware once: `./flash.sh` (needs `arduino-cli`; `brew install arduino-cli`, then `arduino-cli core install arduino:avr` and `arduino-cli lib install Servo`).
2. Start the page: `./bench.sh`. It opens http://localhost:8765 in Chrome. Web Serial only works in Chrome or Edge.
3. Click **Connect board** and pick the Uno. After the first time, the page reconnects by itself when the board is plugged in.

![Browser bench connected to the Uno, pin 9 labelled as the jaw servo](docs/images/bench-connected.webp)

The page draws the Uno, the breadboard and every servo from [`bench/wiring.json`](bench/wiring.json), with each wire in the colour you actually used. Click a servo, a wire or a pin to highlight what belongs together.

- **Parts list**: add a part, set its Uno pin and breadboard row, and tick "planned" until it is wired. Every change is saved straight to `bench/wiring.json`, so commit that file like any other.
- **Servo test**: an angle slider, buttons for the closed and open angle, and "Cycle ×5". "Save slider as closed/open" writes the angle you found into `wiring.json`.
- **Plain pins**: HIGH/LOW, PWM and read, for the motor driver later. **STOP ALL** sets outputs LOW and makes servos hold still.

Breadboard convention: a servo's signal goes in hole `a` of its row and the wire from the Uno in hole `b` of the same row. Leave a free row between parts. Red and brown go into the `+` and `−` strips, which the Uno's 5V and GND feed.

The firmware also works from the Arduino Serial Monitor at 115200 baud: `SERVO 9 80` (jump), `SERVO 9 80 10` (1° every 10 ms), `DETACH 9`, `HIGH 7`, `LOW 7`, `PWM 3 128`, `IN 4`, `PULLUP 4`, `READ 4`, `AREAD A0`, `STOP`, `PING`, `INFO`. Pins 0 and 1 are refused because USB serial uses them. Only pins 3, 5, 6, 9, 10 and 11 do PWM, and 9 and 10 lose PWM while any servo is attached.

## Fabrication

- CAD: Fusion 360
- Slicer: PrusaSlicer
- Printers: Original Prusa MK4S, Prusa Core One
- Material: PLA

PLA settings that work so far: nozzle 220 °C on the first layer and 220–225 °C after; bed 50 °C on the first layer and 50–52 °C after. Organic supports, because snug supports were hard to remove and used a lot of material.

## Open questions

- One controller or two? Sesame's gait firmware on its own ESP32 with the head on a second board, talking over serial, or everything on one board.
- How Sesame's OLED face is mounted, which decides how invasive the head swap is.
- Tail mechanism: direct servo-horn wag, scotch yoke, or something else.
- Ear actuation for the third (sideways) pose, and how many extra servos the lips and tongue need.

## Licence

No licence chosen yet for the original work in this repo. Parts derived from Coglet stay under CC BY-NC 4.0, and parts derived from Sesame under Apache 2.0.
