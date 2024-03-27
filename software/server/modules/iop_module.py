''''''
from typing import Tuple

import numpy as np

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
        try : 
            self.buffer[:-1] = self.buffer[1:]
            self.buffer[-1,:] = data
        except ValueError:
            print("Data shape :", data.shape)
            print("Data : ", data)
            print("Buffer shape :", self.buffer.shape)
            print("Buffer :", self.buffer)
            # exit(1)



    def compute(self) -> Tuple[float]:
        mean = np.mean(self.buffer, axis=0)
        std = np.std(self.buffer, axis=0)

        # print("Mean :", mean)
        # print("Std :", std)

        
        processed = np.square((self.buffer - mean) / std)

        summed = np.sum(processed[-1])

        if self.old_sum == -1:
            self.old_sum = summed
        
        diff = summed - self.old_sum

        self.old_sum = summed

        # summed = summed if summed > 100 else 0

        diff_modif = abs(diff)
        # print("Diff :", diff_modif)

        anorm = np.std(self.buffer[-1])
        return (summed, int(diff_modif>90), anorm)

