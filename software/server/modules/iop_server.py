''' IOPServer class '''

from modules.iop_receiver import IOPReceiver
from modules.iop_sender import IOPSender
from modules.iop_module import IOPModule

import numpy as np
import queue
import threading

class IOPServer:

    def __init__(self, recv_host : str, recv_port : int, sender_host : str, sender_port : int) -> None:
        self.modules : dict[str : IOPModule] = {}
        self.receiver = IOPReceiver(host=recv_host, port=recv_port, callback=self.callback)
        self.sender = IOPSender(host=sender_host, port=sender_port)
        self.queue = queue.Queue()
        self.running = True
        self.thread = threading.Thread(target=self._compute_thread)
        self.thread.start()

        # self.start()
        
    def _compute_thread(self):
        while self.running:
            data = self.queue.get()
            for addr, msg in data.items():
                values = msg.split(' ')
                # print("Values :", values)
                if addr not in self.modules:
                    self.modules[addr] = IOPModule(addr, name=None)
                print(len(values))
                array = np.array(values, dtype=np.float32)
                # print("Array :", array)
                # print("Array shape :", array.shape)
                self.modules[addr].add_data(array)
                result = self.modules[addr].compute()
                self.sender.send(result)


    def callback(self, data : dict) -> None:        
        self.queue.put(data)
    
    def stop(self) -> None:
        self.running = False
        self.receiver.stop()
        self.sender.stop()
        self.thread.join(1)
    
    def start(self) -> None:
        self.receiver.start()
        self.sender.start()
            

    

