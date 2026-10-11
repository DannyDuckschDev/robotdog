"""Serves the browser bench on localhost and lets it save wiring.json.

Plain static file server for this folder, plus PUT /wiring.json so edits
made in the page land in the repo. Only listens on 127.0.0.1.
"""

import json
import sys
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

HERE = Path(__file__).resolve().parent
WIRING = HERE / "wiring.json"
MAX_BYTES = 256 * 1024


class BenchHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header("Cache-Control", "no-store")
        super().end_headers()

    def do_PUT(self):
        if self.path.split("?")[0] != "/wiring.json":
            self.send_error(404)
            return
        length = int(self.headers.get("Content-Length", 0))
        if length <= 0 or length > MAX_BYTES:
            self.send_error(413)
            return
        try:
            data = json.loads(self.rfile.read(length))
        except ValueError:
            self.send_error(400, "not valid JSON")
            return
        WIRING.write_text(json.dumps(data, indent=2) + "\n")
        self.send_response(204)
        self.end_headers()


if __name__ == "__main__":
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 8765
    handler = partial(BenchHandler, directory=str(HERE))
    print(f"Bench running at http://localhost:{port} (Ctrl+C to stop)")
    ThreadingHTTPServer(("127.0.0.1", port), handler).serve_forever()
