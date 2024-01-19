# from future import annotations

from modules.iop_server import IOPServer

# Fetching ip address

RECV_HOST = '192.168.10.111'
RECV_PORT = 3000

SENDER_HOST = RECV_HOST
SENDER_PORT = 4000
   
       


if __name__ == '__main__':
    server = IOPServer(RECV_HOST, RECV_PORT, SENDER_HOST, SENDER_PORT)
    while True:
        pass