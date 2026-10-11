#!/bin/sh
# Compiles a sketch and uploads it to the first Uno found over USB.
# Usage: ./flash.sh                     (bench firmware)
#        ./flash.sh tests/jaw_test      (any sketch folder)
# Needs arduino-cli (brew install arduino-cli; arduino-cli core install arduino:avr;
# arduino-cli lib install Servo).
cd "$(dirname "$0")"
SKETCH=${1:-firmware/bench}
PORT=$(ls /dev/cu.usbmodem* /dev/cu.usbserial* /dev/cu.wchusbserial* 2>/dev/null | head -1)
[ -z "$PORT" ] && { echo "No board found. Check the USB cable (some are charge-only)."; exit 1; }
echo "Flashing $SKETCH to $PORT"
arduino-cli compile --fqbn arduino:avr:uno "$SKETCH" &&
arduino-cli upload -p "$PORT" --fqbn arduino:avr:uno "$SKETCH"
