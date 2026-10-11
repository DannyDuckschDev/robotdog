#!/bin/sh
# Compiles firmware/bench and uploads it to the first Uno found over USB.
# Needs arduino-cli (brew install arduino-cli; arduino-cli core install arduino:avr).
cd "$(dirname "$0")"
PORT=$(ls /dev/cu.usbmodem* /dev/cu.usbserial* /dev/cu.wchusbserial* 2>/dev/null | head -1)
[ -z "$PORT" ] && { echo "No board found. Check the USB cable (some are charge-only)."; exit 1; }
echo "Flashing $PORT"
arduino-cli compile --fqbn arduino:avr:uno firmware/bench &&
arduino-cli upload -p "$PORT" --fqbn arduino:avr:uno firmware/bench
