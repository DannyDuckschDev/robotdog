# Servo calibration

Angles found with the test sketches. Fill a row in once the servo is mounted in the mechanism, since the limits depend on how the horn is fitted.

If a servo buzzes or strains at an angle, it has hit a mechanical stop. Back off a few degrees and use that as the limit.

| Servo | Uno pin | Min angle | Max angle | Neutral | Notes |
|---|---|---|---|---|---|
| Mouth (jaw) | 9 | | | | min = closed, max = open |
| Yaw (neck) | 2 | | | | |
| Pitch (neck) | 3 | | | | |
| Roll (neck) | — | | | | not used for now |
| Left eye | 6 | 55 | 105 | 80 | tested with `tests/eyes/eye_sweep` |
| Right eye | 5 | 35 | 115 | 80 | row 19, mirrored; 45° below the center, 35° above |
| Eyelid | 7 | | | | |
| Left ear | 8 | | | | |
| Right ear | 10 | | | | |
