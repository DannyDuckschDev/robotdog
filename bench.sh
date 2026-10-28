#!/bin/sh
# Starts the browser bench at http://localhost:8765 and opens it in Chrome.
# Web Serial only works in Chrome or Edge. Stop with Ctrl+C.
# Changes made in the page are saved to bench/wiring.json.
cd "$(dirname "$0")"
URL=http://localhost:8765
( sleep 1; open -a "Google Chrome" "$URL" 2>/dev/null || open -a "Microsoft Edge" "$URL" 2>/dev/null || open "$URL" ) &
exec python3 bench/server.py 8765
