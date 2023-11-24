import ctypes
import matplotlib as plt
import numpy as np

my_lib = ctypes.CDLL("./iop_lib.so")


if __name__ == '__main__':

    array = [3.2,23.3,6.0,231]
    size = len(array)
    c_float_array = (ctypes.c_float*size)(*array)
    print(c_float_array)
    result = my_lib.sum_custom(c_float_array, size)
    print(type(result))