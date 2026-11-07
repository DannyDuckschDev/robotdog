#!/usr/bin/env python3
"""Log the Uno's serial output to a CSV file, with phase labels you type live.

Replaces screenshots of the Serial Monitor: every reading is saved with a
timestamp and the phase you were in (e.g. "40", "120", "fast1").

Usage (from the repo folder, in Terminal):

    python3 tools/serial_log.py range-test

While it runs:
    type a label + Enter   start a new phase, e.g.  40  /  120  /  fast1  /  slow
    just Enter             mark a transition (walking between marks): phase "-"
    q + Enter              stop and save

The file lands in data/sensor-logs/<date>_<time>_<name>.csv.
Then plot it:  python3 tools/plot_log.py data/sensor-logs/<file>.csv

Needs pyserial once:  pip3 install pyserial
Close the Arduino IDE's Serial Monitor (and the browser bench) first:
only one program can use the USB port at a time.
"""

import argparse
import csv
import glob
import os
import sys
import threading
import time
from datetime import datetime

try:
    import serial
except ImportError:
    sys.exit("pyserial is missing. Install it with:  pip3 install pyserial")

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LOG_DIR = os.path.join(REPO, "data", "sensor-logs")


def find_port():
    ports = sorted(glob.glob("/dev/cu.usbmodem*") + glob.glob("/dev/cu.usbserial*")
                   + glob.glob("/dev/ttyACM*") + glob.glob("/dev/ttyUSB*"))
    if not ports:
        sys.exit("No Uno found. Is the USB cable plugged in? Or pass --port.")
    return ports[0]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("name", nargs="?", default="log", help="short name for the file, e.g. range-test")
    ap.add_argument("--port", help="serial port (default: first usbmodem/usbserial found)")
    ap.add_argument("--baud", type=int, default=9600, help="baud rate, must match Serial.begin() (default 9600)")
    args = ap.parse_args()

    port = args.port or find_port()
    os.makedirs(LOG_DIR, exist_ok=True)
    stamp = datetime.now().strftime("%Y-%m-%d_%H%M")
    path = os.path.join(LOG_DIR, f"{stamp}_{args.name}.csv")

    try:
        ser = serial.Serial(port, args.baud, timeout=0.5)
    except serial.SerialException as e:
        sys.exit(f"Cannot open {port}: {e}\nClose the Serial Monitor / browser bench and try again.")

    phase = {"label": "-"}
    stop = threading.Event()

    def read_keyboard():
        for line in sys.stdin:
            text = line.strip()
            if text.lower() == "q":
                stop.set()
                return
            phase["label"] = text or "-"
            print(f"--- phase: {phase['label']} ---")

    threading.Thread(target=read_keyboard, daemon=True).start()

    # Opening the port resets the Uno; give it a moment, then drop half-received lines.
    time.sleep(2)
    ser.reset_input_buffer()
    t0 = time.monotonic()
    rows = 0

    print(f"Logging {port} at {args.baud} baud to {os.path.relpath(path, REPO)}")
    print("Type a phase label + Enter (e.g. 40, 120, 200, fast1, slow). Enter alone = transition. q + Enter = stop.\n")

    with open(path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["t_s", "clock", "phase", "value"])
        try:
            while not stop.is_set():
                raw = ser.readline().decode("utf-8", errors="replace").strip()
                if not raw:
                    continue
                try:
                    value = float(raw.split(",")[0])
                except ValueError:
                    print(f"(text) {raw}")
                    continue
                t = time.monotonic() - t0
                w.writerow([f"{t:.3f}", datetime.now().strftime("%H:%M:%S.%f")[:-3], phase["label"], value])
                rows += 1
                if rows % 10 == 0:
                    f.flush()
                print(f"{t:7.2f} s  [{phase['label']:>6}]  {value:8.2f}")
        except KeyboardInterrupt:
            pass
    ser.close()
    print(f"\nSaved {rows} readings to {os.path.relpath(path, REPO)}")
    print(f"Plot:  python3 tools/plot_log.py {os.path.relpath(path, REPO)}")


if __name__ == "__main__":
    main()
