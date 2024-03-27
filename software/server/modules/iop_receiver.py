''' IOPReceiver class'''

from typing import Callable

import queue
import socket
import threading

class IOPReceiver:
    
        def __init__(self, *, host : str, port : int, callback : Callable) -> None:
            self.host = host
            self.port = port
            self.callback = callback
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
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
                    print("Length data", len(data))
                    decoded_data = data.decode('ascii')
                    if decoded_data.startswith('#') and decoded_data.endswith(';\n'):
                        decoded_data = decoded_data[1:-2]
                        print("data", decoded_data)
                    # self.msg_queue.put({addr : data.decode('ascii')})
                        # print('Received data :', decoded_data)
                        self.callback({addr : decoded_data})
                    else:
                        print('Received data is not valid')
                        
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
