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

## 7. Lower lip (not working yet)

- **What:** Fifth servo on Uno pin 8, breadboard row 25, with `tests/lower_lip/lower_lip_sweep`: 20° up and 20° down around a center of 90°. Separate from the jaw on pin 9. The left ear's planned pin moved from 8 to 4.
- **Status:** The sketch compiles, but the lip does not move yet.
- **Open question:** The lower lip is fixed to the head with a screw and has more resistance than the other parts. Check whether an SG90 (about 1.8 kg·cm) is strong enough, or whether it needs a stronger servo such as the metal-gear MG90S (about 2.2 kg·cm, same size). First check: loosen the screw a little and see whether the lip moves by hand without force.
- **Next:** A stronger servo is on hand and will be tested on the lower lip.
- **Error along the way:** Upload failed with `cannot open port /dev/cu.usbmodem…: Resource busy`. The browser bench was still connected to the Uno over Web Serial, and only one program can use the port at a time. Fix: click Disconnect in the bench (or close its tab) before uploading a sketch.
