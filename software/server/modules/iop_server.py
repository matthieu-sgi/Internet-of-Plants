''' IOPServer class '''

from modules.iop_receiver import IOPReceiver
from modules.iop_sender import IOPSender
from modules.iop_module import IOPModule

import numpy as np


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
            

    

