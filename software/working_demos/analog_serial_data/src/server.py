# wait for a client to connect to a socket

import socket

if __name__ == '__main__':
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('172.21.72.178', 13000))

        s.listen()
        conn, addr = s.accept()

        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(1024)
                print(data)
                if not data:
                    break
                conn.sendall(data)