# from future import annotations

from typing import Callable

import numpy as np
import socket
import threading
import queue

# Fetching ip address

RECV_HOST = '192.168.10.111'
RECV_PORT = 3000

SENDER_HOST = '192.168.10.111'
SENDER_PORT = 4000
   

class IOPReceiver:
    
        def __init__(self, *, host : str, port : int, callback : Callable) -> None:
            self.host = host
            self.port = port
            self.callback = callback
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.bind((self.host, self.port))
            self.socket.listen(5)
            self.clients = []
            self.receiving_thread = threading.Thread(target=self._receiving_thread)
            self.running = True
            self.start()
            self.msg_queue = queue.Queue() 
    
        # Receiving thread that put each receiving socket in a thread
        def _receiving_thread(self) -> None:
            print('Receiving thread started')
            while self.running:
                print('Waiting for connection')
                client_socket, addr = self.socket.accept()
                print('Got connection from', addr)
                client_thread = threading.Thread(target=self._client_thread, args=(client_socket,addr))

                client_thread.start()
                self.clients.append((client_socket, client_thread))
        
        def _client_thread(self, client_socket : socket.socket, addr : str) -> None:
            print('Client thread started')
            while self.running:
                data = client_socket.recv(1024)
                if data:
                    print(data)
                    # self.msg_queue.put({addr : data.decode('ascii')})

                    self.callback({addr : data.decode('ascii')})
                else:
                    break
            client_socket.close()
    
        def start(self) -> None:
            self.receiving_thread.start()
        
        def stop(self) -> None:
            self.running = False
            for client_socket, client_thread in self.clients:
                client_socket.close()
                client_thread.join(timeout=1)
            self.receiving_thread.join(timeout=1)
    
        def close(self) -> None:
            self.socket.close()
        
        def get_msg(self) -> dict:
            return self.msg_queue.get()


class IOPModule:
    
    def __init__(self, addr : str, name : str, *, buffer_size = 400, min_freq = 100, max_freq = 300) -> None:
        self.addr = addr
        self.name = name
        self.buffer_size = buffer_size
        self.min_freq = min_freq
        self.max_freq = max_freq
        self.buffer = np.zeros((buffer_size, self.max_freq - self.min_freq), dtype=np.float32)
        self.old_sum = -1

    def get_addr(self) -> int:
        return self.addr
    
    def get_name(self) -> str:
        return self.name
    
    def add_data(self, data : np.ndarray) -> None:
        # Shift buffer and add data
        self.buffer[:-1] = self.buffer[1:]
        self.buffer[-1:] = data


    def compute(self) -> tuple[float]:
        mean = np.mean(self.buffer, axis=0)
        std = np.std(self.buffer, axis=0)

        processed = np.square((self.buffer - mean) / std)

        summed = np.sum(processed[-1], axis=0)

        if self.old_sum == -1:
            self.old_sum = summed
        
        diff = summed - self.old_sum

        self.old_sum = summed

        return (diff, summed)


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
        self.socket.send('Hello'.encode('ascii'))
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
        self.sending_queue.put(msg.encode('utf-8'))

    def start(self) -> None:
        self.sending_thread.start()
    
    def stop(self) -> None:
        self.running = False
        self.sending_thread.join(timeout=1)


    def close(self) -> None:
        self.socket.close()
    


class IOPServer:

    def __init__(self, recv_host : str, recv_port : int, sender_host : str, sender_port : int) -> None:
        self.modules : dict[str : IOPModule] = {}
        self.receiver = IOPReceiver(host=recv_host, port=recv_port, callback=self.callback)
        self.sender = IOPSender(host=sender_host, port=sender_port)
        # self.start()


    def callback(self, data : dict) -> None:
        for addr, msg in data.items():
            values = msg.split(' ')
            print("Values :", values)
            if addr not in self.modules:
                self.modules[addr] = IOPModule(addr, name=None)
            array = np.array(values, dtype=np.float32)
            print("Array :", array)
            print("Array shape :", array.shape)
            self.modules[addr].add_data(array)
            result = self.modules[addr].compute()
            self.sender.send(result)
    
    def stop(self) -> None:
        self.receiver.stop()
        self.sender.stop()
    
    def start(self) -> None:
        self.receiver.start()
        self.sender.start()
            

    



        

        


if __name__ == '__main__':

    server = IOPServer(RECV_HOST, RECV_PORT, SENDER_HOST, SENDER_PORT)
    # server.start()
    while True:
        pass
    ...