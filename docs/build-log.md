# Build log

What was done, in order, and why. One entry per step that worked. Details and photos are in [`setup-guide.md`](setup-guide.md).

## 1. Repo for code, tests and notes

- **What:** A Git repo with one folder per test sketch, a calibration sheet and a README.
- **Why:** Test sketches, angles and wiring change often. In a repo every state is saved and can be restored.
- **How:** Each test is its own Arduino sketch (a folder with an `.ino` file of the same name). The Uno runs one sketch at a time; uploading replaces the previous one.

## 2. First servo: the jaw

- **What:** SG90 servo on Uno pin 9, through the breadboard, tested with `tests/jaw/jaw_test` and `jaw_cycle`.
- **Why:** Start with one servo to learn wiring, uploading and the Serial Monitor before adding more.
- **How:** Brown to `−`, red to `+`, orange to an Uno pin. The `+`/`−` strips only carry power once something feeds them. Angles found: closed 0°, open 80°.

## 3. Separate power for the servos

- **What:** A USB cable with jumper ends (red `+`, blue `−`) feeds the strips. The wire from Uno `5V` to the strip was removed; Uno `GND` stays on the `−` strip.
- **Why:** Two servos draw more current than the Uno's 5V pin can give. Only one supply may feed a strip, and the servo signal needs the Uno's ground as reference.
- **How:** Uno on the Mac for code and data, the power cable in a USB port or charger. If the Uno disconnects when a servo moves, the supply is too weak: use a phone charger.

## 4. Neck pitch (nodding)

- **What:** Second servo on Uno pin 3, breadboard row 9, tested with `tests/neck/neck_nod`.
- **Why:** The neck only has a small range, so it moves slowly, 1° at a time, around a center.
- **How:** `STEP_DELAY` sets the speed, `RANGE` the angle each way. Range so far: 45° to 135°. Tests are sorted by body part (`tests/jaw`, `tests/neck`, …).
