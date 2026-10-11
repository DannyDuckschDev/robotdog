#!/bin/sh
# Starts the browser bench at http://localhost:8765 and opens it in Chrome.
# Web Serial only works in Chrome or Edge. Stop with Ctrl+C.
cd "$(dirname "$0")"
URL=http://localhost:8765
( sleep 1; open -a "Google Chrome" "$URL" 2>/dev/null || open -a "Microsoft Edge" "$URL" 2>/dev/null || open "$URL" ) &
echo "Bench running at $URL (Ctrl+C to stop)"
exec python3 -m http.server 8765 --bind 127.0.0.1 --directory bench
