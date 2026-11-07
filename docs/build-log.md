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

## 5. Left eye (looking left and right)

- **What:** Third servo on Uno pin 6, breadboard row 15, tested with `tests/eyes/eye_sweep`.
- **Why:** The eye turns through a gear and a curved rack, so it starts with a small range to check that the mechanism moves freely before going further.
- **How:** First tested on the neck's wires (pin 3, row 9), then given its own row and pin. Center 80°, range ±25° (55° to 105°). Type `r` in the Serial Monitor to release the servo if it hums without moving: a blocked servo gets hot.

## 6. Right eye

- **What:** Fourth servo on Uno pin 5, breadboard row 19, tested with `tests/eyes/eye_sweep_right`.
- **Why:** The right eye is mounted mirror-image to the left one, so the same angle can turn it the other way. The sketch has a `MIRRORED` switch so both eyes look the same way first.
- **How:** Started like the left eye (center 80°, 55° to 105°), then widened per side, because the eye turns further one way than the other. Both eye sketches now have a separate range for each side (`RANGE_BELOW`, `RANGE_ABOVE`) and print the angle at each end. Right eye: 45° below, 35° above the center (35° to 115°).
- **Error along the way:** The upload failed with `cannot open port /dev/cu.usbmodem…: No such file or directory`. The sketch had compiled fine; the Mac simply did not see the Uno, because the USB cable was not fully plugged in. Fix: push the cable in until the Uno's ON LED lights, then pick the `usbmodem` port again under Tools → Port (the number can change after replugging).

## 7. Lower lip

- **What:** Fifth servo on Uno pin 8, breadboard row 25, tested with `tests/lower_lip/lower_lip_sweep`. Separate from the jaw on pin 9. The left ear's planned pin moved from 8 to 4.
- **Why:** The lip should rest closed and only open from there, so the sketch starts at the closed angle, opens slowly and closes again. It never moves past closed.
- **How:** Closed 165°, open 80°. The closed angle is the bigger one because of how the servo is mounted. Type an angle in the Serial Monitor to fine-tune both values.
- **First attempt:** A sweep of ±20° around 90°. The servo only hummed and the lip did not move. With the power off, neither could be turned by hand, so something was jammed. The original Coglet uses the same kind of servo, so the SG90 should be strong enough once the lip moves freely. A sweep around a center also closed the lip further first before opening it, which is why the sketch now goes closed → open → closed.
- **Still open:** The lip jumps at the start, because a servo cannot report its position and goes to the closed angle at full speed. It also jerks while moving, likely friction at the pivot screw, or gear slack at the turnaround. Before unplugging, leave the lip closed so the next start has nothing to jump. A stronger servo is on hand as a backup, but not needed for now. The mouth code is to be refactored later ([issue #4](https://github.com/DannyDuckschDev/robotdog/issues/4)).
- **Error along the way:** Upload failed with `cannot open port /dev/cu.usbmodem…: Resource busy`. The browser bench was still connected to the Uno over Web Serial, and only one program can use the port at a time. Fix: click Disconnect in the bench (or close its tab) before uploading a sketch.
- **Error along the way:** Upload failed with `Device not configured` and `programmer is not responding`, ten attempts. The Mac had lost the Uno partway through because the USB cable was not fully plugged in. Fix: push the cable in until the ON LED lights and pick the `usbmodem` port again.

## 8. Neck base (turning left and right)

- **What:** Yaw servo in the Coglet base on Uno pin 10, breadboard row 30, tested with `tests/neck/neck_base`. The right ear's planned pin moved from 10 to 2.
- **Why:** The base turns the whole head, so the sketch moves it 1° every 20 ms and never leaves a safe range.
- **How:** Type an angle in the Serial Monitor, or `+` / `-` for 2° steps. Range 45° to 135° around a center of 90°, and that range works well.

## 9. First sensor: ultrasonic distance (HC-SR04)

- **What:** HC-SR04 on Trig `A0`, Echo `A1`, VCC to Uno `5V`, GND to the `−` strip. Tested with `tests/sensors/ultrasonic_distance`. Details and photos: [`interaction-sensors.md`](interaction-sensors.md).
- **Why:** the dog had only outputs so far. How someone approaches is the core of the artwork, and this is the cheapest sensor that measures it.
- **How:** analog pins work as digital pins, which was needed because the digital header is full. Readings are stable to ±1–2 cm with occasional large outliers (about 241 cm), so the next step is a median filter before computing approach speed.
- **Error along the way:** "nothing happens" after upload: the readings are in the Serial Monitor, not the Output panel.
- **Range test:** stable to about 90 cm, usable to about 160 cm in this room. Fast approaches (~1.1 m/s) and slow ones (~0.15 m/s) differ by a factor of ~8, so speed is a reliable input. Full table in [`interaction-sensors.md`](interaction-sensors.md#3-range-and-speed-test-in-the-room).
