from poker import Deck, PokerHand
import json
import random
from http.server import BaseHTTPRequestHandler, HTTPServer

class SimpleRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # Generate some random-ish example content
        content = {
            "random_number": random.randint(1, 100),
            "random_string": ''.join(random.choices('abcdefghijklmnopqrstuvwxyz', k=10)),
            "random_float": random.random(),
        }

        # Send response headers
        self.send_response(200)
        self.send_header("Content-type", "application/json")
        self.end_headers()

        # Send the JSON content
        self.wfile.write(json.dumps(content).encode())

def run(server_class=HTTPServer, handler_class=SimpleRequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()
