''''''

import queue
import socket
import threading


class IOPSender:

    def __init__(self, *, host : str, port : int) -> None:
        self.host = host
        self.port = port
        # print('Connecting to', self.host, self.port)
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # while True:
            # try :
                # self.socket.connect((self.host, self.port))
                # break
            # except ConnectionRefusedError:
                # print('Connection refused')

        self.socket.connect((self.host, self.port))
        print('Connected to', self.host, self.port)
        self.socket.send('Hello;\n'.encode('ascii'))
        print('Sent hello')
        self.sending_queue = queue.Queue()
        self.sending_thread = threading.Thread(target=self._sending_thread)
        self.running = True
        self.start()

    def _sending_thread(self) -> None:
        while self.running:
            data = self.sending_queue.get()
            self.socket.send(data)
        
    def send(self, data : tuple[float]) -> None:
        msg = ' '.join([str(x) for x in data])
        msg += ';\n'
        # print(msg)
        self.sending_queue.put(msg.encode())

    def start(self) -> None:
        self.sending_thread.start()
    
    def stop(self) -> None:
        self.running = False
        self.sending_thread.join(timeout=1)


    def close(self) -> None:
        self.socket.close()
    

