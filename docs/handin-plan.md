# Hand-in plan: two weeks

Plan for handing in both modules (Digital Fabrication and Physical Interfaces II) about two weeks from now, around **Thu 19 Nov 2026**. Adjust the dates once the real deadlines are confirmed.

## Scope for the hand-in

Two weeks is not enough to build Sesame, make it walk, design a tail and integrate everything. So the hand-in shows **the head as an interactive prototype**, on a laser-cut base, reacting to people. Walking, the tail and full integration are documented as the next phase.

The demo:

- The dog notices someone approaching (ultrasonic sensor).
- How it reacts depends on **how** the person approaches. Slow means relaxed; fast or too close means alert, then afraid (ears back, mouth closed, head turned away).
- Gentle petting (foil touch sensor) brings it back to relaxed.
- A soft sound (passive buzzer) supports each state.

This is the artwork's message as an interaction: how you approach a dog changes how it feels.

## Must-haves

If time runs short, these stay. Everything else is cut first.

### Both modules

- [ ] Confirm the real deadline and hand-in format for **each** module (Physical Interfaces: PDF portfolio of 2000 words with the word count on the cover, a 500-word self-assessment, an optional video of up to 10 minutes, and a mandatory demo day; Digital Fabrication: format still to check)
- [ ] Interaction concept: one page with a state diagram (relaxed, alert, afraid, frozen, back to relaxed) and which input triggers which state
- [ ] Ear servos tested and calibrated (the ears carry most of the body language)
- [ ] Jaw servo calibrated on hardware
- [ ] Ultrasonic sensor working, measuring distance and speed of approach
- [ ] One sketch that runs the state machine: sensor in, poses out
- [ ] User test with 3–5 people (see Evaluation)
- [ ] Photos and short videos of every step, all taken yourself

### Digital Fabrication

- [ ] **Original Fusion part from scratch:** a mount for the ultrasonic sensor (two Ø16 mm holes, 45 × 20 mm board), placed on the chest or below the head. Use user parameters for the tolerances
- [ ] **PETG part:** inner neck / top base ([#5](https://github.com/DannyDuckschDev/robotdog/issues/5)), with the PETG reasoning written down
- [ ] **Laser-cut part:** a base or plinth that holds the head, the Uno, the breadboard and the power supply, with a cable path and the sensor mount. Book laser cutter time now
- [ ] Material log: part, material, weight from PrusaSlicer, print time, failed prints and why
- [ ] A short CAM overview: FDM (PrusaSlicer), laser (DXF, kerf, vector vs. raster) and CNC (Fusion Manufacture, toolpaths)
- [ ] A short scaling reflection: what would change for a small series or injection moulding
- [ ] Assessment criteria for the use context: exhibition space, visitors' hands, children, safety and pinch points, noise, cable vs. battery

### Physical Interfaces II

- [ ] Wizard-of-Oz test: you trigger the poses remotely while a person approaches, before the sensor logic is finished
- [ ] Evaluation of the sensor version: a short questionnaire adapted from TAM2, or a reasoned alternative
- [ ] Theory: Dourish, *Where the Action Is*; van den Hoven, Mazalek & Margetis, *Tangible and Embodied Interaction*; the process framed as Research through Design
- [ ] Good practice: privacy (no camera, or no stored images), accessibility (sound as a non-visual cue, height for wheelchair users and children), safety
- [ ] Primary research collected: the earlier artwork, the dog-behaviour analysis, the cardboard prototype
- [ ] Portfolio PDF, written as a document (not exported slides), with numbered, captioned figures and correct citations
- [ ] 500-word self-assessment with an estimate of time spent

## Nice-to-have

Only if the must-haves are done:

- [ ] Foil touch sensor for petting (could move up to must-have; it is cheap and makes the piece tangible)
- [ ] PIR sensor to wake the dog when someone enters the room
- [ ] Sound sensor: a loud voice or clap startles the dog
- [ ] TPU wall-thickness test (0.8 / 1.2 / 2 mm) for ears and lips
- [ ] Eyelid servo
- [ ] Video of up to 10 minutes

Postponed after the hand-in: Sesame body and walking, the tail mechanism, the ESP32 and PCA9685, the ear's third (sideways) pose, separate upper lip, tongue.

## Parts from the kit

| Part | Use | Pin (proposal) |
|---|---|---|
| Ultrasonic sensor HC-SR04 | Distance and speed of approach | Trig A0, Echo A1 |
| Aluminium foil + 1 MΩ resistor | Touch, petting (CapacitiveSensor library) | Send 11, receive 12 |
| Passive buzzer | Soft sounds per state | 13 |
| IR remote + receiver | Wizard-of-Oz control during the user test | A2 |
| PIR motion sensor HC-SR501 | Nice-to-have: someone entered | A3 |
| Sound sensor | Nice-to-have: loud noise | A4 (digital output) |

Pins 2 to 10 are taken by the head servos (see the pin map in the README). Analog pins A0 to A5 also work as digital pins.

The IR library and the buzzer (`tone()`) both use the same timer, so they conflict. That is fine, because IR is only needed for the Wizard-of-Oz test.

Not used: the LCD and the 7-segment and dot-matrix displays (the module is about non-screen interaction), plus the stepper motor, the DC motor, RFID and the DHT11.

## Day by day

| Day | Do |
|---|---|
| **Thu 5 Nov** | Confirm deadlines and formats. Book the laser cutter. Draft the interaction concept and state diagram |
| **Fri 6** | Test and calibrate both ear servos (pins 4 and 2). Calibrate the jaw. Test the ultrasonic sensor on its own |
| **Sat 7** | Fusion: model the sensor mount from scratch. Test-print it in PLA |
| **Sun 8** | Print the PETG inner neck (#5). Start the material log. Write down the PETG reasoning |
| **Mon 9** | State-machine sketch: ultrasonic sensor driving the ears, eyes, neck and lip |
| **Tue 10** | Wizard-of-Oz setup with the IR remote. Design the laser-cut base in Fusion and export the DXF |
| **Wed 11** | **User test 1** (Wizard of Oz, 3–5 people). Notes and photos |
| **Thu 12** | Laser-cut the base. Mount the head, electronics and sensor |
| **Fri 13** | Integrated test. Foil touch and buzzer if time allows. **User test 2** with the sensor version and the questionnaire |
| **Sat 14** | Write: Physical Interfaces portfolio draft |
| **Sun 15** | Write: Digital Fabrication documentation draft (CAM overview, materials, scaling reflection) |
| **Mon 16** | Theory and citations. 500-word self-assessment. Number and caption the figures |
| **Tue 17** | Photos and video. Rehearse the demo |
| **Wed 18** | Buffer. Proofread. Export the PDFs with the word count on the cover |
| **Thu 19** | Hand in |

## Evaluation

**User test 1: Wizard of Oz, before the sensors**

- 3–5 people, one at a time.
- The person approaches the head.
- You trigger the poses with the IR remote.
- Afterwards ask:
  - Which feeling did each pose show?
  - Did anything feel threatening?
  - What made you want to come closer, or step back?

**User test 2: sensor version**

- Same task, but the dog reacts by itself.
- A short questionnaire on a 1–5 scale, adapted from TAM2:
  - Was it clear how the dog felt?
  - Was it easy to know how to approach it?
  - Would you want to interact with it again?
- One open question: what would you change?

Ask before taking photos of participants, and keep the answers anonymous.
