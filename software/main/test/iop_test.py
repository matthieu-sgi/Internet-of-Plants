import ctypes
import matplotlib as plt
import numpy as np

my_lib = ctypes.CDLL("./libiop.so")

SIZE = 200

def testing_ema(data):
    ema = np.zeros(SIZE)
    print(ema)
    my_lib.ema.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_int), ctypes.c_int]
    # my_lib.ema.restype = ctypes.POINTER(ctypes.c_float)

    c_int_array = (ctypes.c_int*SIZE)(*data)
    c_float_array = (ctypes.c_float*SIZE)(*ema)
    print(f'c_float_array {list(c_float_array)}')
    print(f'size of c_float_array {len(c_float_array)}')
    my_lib.ema(c_float_array, c_int_array, SIZE)
    print(list(c_float_array))

def testing_normalization(data):
    ema = np.random.randint(0,1024,SIZE)
    std = np.random.randint(0,1024,SIZE)
    result = np.zeros(SIZE)
    # print(ema)
    my_lib.ema_std.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
    my_lib.ema.restype = None

    c_int_array = (ctypes.c_int*SIZE)(*data)
    c_float_array = (ctypes.c_float*SIZE)(*ema)
    c_float_array_std = (ctypes.c_float*SIZE)(*std)
    c_float_array_result = (ctypes.c_float*SIZE)(*result)


    result = my_lib.ema_std(c_int_array, SIZE, c_float_array, c_float_array_std, c_float_array_result)
    # print(ema)
    # print(std)

    print(result)

def testing_basic():
    data = [1,2]
    size = len(data)
    my_lib.basic_function.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
    # c_int_array = (ctypes.c_int*size)(*data)
    # print(c_int_array)

    c_float_array = (ctypes.c_float*size)(*data)
    my_lib.basic_function(c_float_array, size)
    print(c_float_array)
    print(f'convert to list {list(c_float_array)}')
    print(f'data {data}')
    # print(result)

def testing_sum(data):
    print(np.sum(data))
    # call the sum_custom function that is taking the array of float and the size of the array

    # my_lib.sum_custom.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
    my_lib.sum_custom.restype = ctypes.c_float

    c_float_array = (ctypes.c_float*SIZE)(*data)
    result = my_lib.sum_custom(c_float_array, SIZE)
    print(result)


if __name__ == '__main__':

    data = np.random.randint(0,1024, size= SIZE) # Representing the 200 freq from 100khz to 300khz
    # testing_ema(data)
    # testing_normalization(data)
    # testing_sum(data)
    testing_basic()
    # thalia






    # size = len(data)

    # c_float_array = (ctypes.c_float*size)(*array)
    # print(c_float_array)
    # result = my_lib.sum_custom(c_float_array, size)
    # print(type(result))
    # print(result)