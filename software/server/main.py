# from future import annotations

from modules.iop_server import IOPServer


import signal
import sys
# Fetching ip address

RECV_HOST = '0.0.0.0'
RECV_PORT = 3000

SENDER_HOST = 'localhost'
SENDER_PORT = 4000
   
       
def signal_handler(sig, frame):
    print('You pressed Ctrl+C!')
    sys.exit(0)

if __name__ == '__main__':
    server = IOPServer(RECV_HOST, RECV_PORT, SENDER_HOST, SENDER_PORT)
    signal.signal(signal.SIGTERM, server.signal_handler)
    while True:
        pass