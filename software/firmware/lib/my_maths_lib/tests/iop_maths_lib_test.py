import ctypes
import time
import matplotlib as plt
import numpy as np

my_lib = ctypes.CDLL("./iop_maths_lib.so")

SIZE = 200
BUFFER_SIZE = 100
ALPHA = 1 - 1/BUFFER_SIZE
buffer = np.zeros((BUFFER_SIZE,SIZE))


def testing_ema(data, ema : list) -> list:
    # ema = np.zeros(SIZE)
    # alpha = 0.99 # 1 - 1/100
    my_lib.ema_update.argtypes = [ctypes.POINTER(ctypes.c_float),
                           ctypes.POINTER(ctypes.c_int),
                           ctypes.c_int,
                           ctypes.c_float]
    # my_lib.ema.restype = ctypes.POINTER(ctypes.c_float)

    c_float_array = (ctypes.c_float*SIZE)(*ema)
    c_int_array = (ctypes.c_int*SIZE)(*data)
    c_float_alpha = ctypes.c_float(ALPHA)
    my_lib.ema_update(c_float_array, c_int_array, SIZE, c_float_alpha)
    # ema = list(c_float_array)
    return list(c_float_array)

def testing_ema_std(data, ema_std, ema) -> list:
    my_lib.ema_std_update.argtypes = [ctypes.POINTER(ctypes.c_int),
                                      ctypes.c_int,
                                      ctypes.POINTER(ctypes.c_float),
                                      ctypes.POINTER(ctypes.c_float)]
    c_int_array = (ctypes.c_int*SIZE)(*data)
    c_float_array = (ctypes.c_float*SIZE)(*ema)
    c_float_array_std = (ctypes.c_float*SIZE)(*ema_std)

    my_lib.ema_std_update(c_int_array, SIZE, c_float_array_std, c_float_array)

    return list(c_float_array_std)

def testing_normalization(data):
    ema = np.random.randint(0,1024,SIZE)
    std = np.random.randint(0,1024,SIZE)
    result = np.zeros(SIZE)
    my_lib.ema_std.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]

    c_int_array = (ctypes.c_int*SIZE)(*data)
    c_float_array = (ctypes.c_float*SIZE)(*ema)
    c_float_array_std = (ctypes.c_float*SIZE)(*std)
    c_float_array_result = (ctypes.c_float*SIZE)(*result)


    result = my_lib.ema_std(c_int_array, SIZE, c_float_array, c_float_array_std, c_float_array_result)

    print(result)

def testing_avg(data) -> float:
    # call the sum_custom function that is taking the array of float and the size of the array

    my_lib.average.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
    my_lib.average.restype = ctypes.c_float

    c_float_array = (ctypes.c_float*SIZE)(*data)
    result = my_lib.average(c_float_array, SIZE)
    return float(result)

def testing_basic():
    data = [1,2]
    size = len(data)
    my_lib.basic_function.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
    # c_int_array = (ctypes.c_int*size)(*data)

    c_float_array = (ctypes.c_float*size)(*data)
    my_lib.basic_function(c_float_array, size)

def testing_sum(data) -> float:
    # call the sum_custom function that is taking the array of float and the size of the array

    # my_lib.sum_custom.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
    my_lib.sum_custom.restype = ctypes.c_float

    c_float_array = (ctypes.c_float*SIZE)(*data)
    result = my_lib.sum_custom(c_float_array, SIZE)
    return float(result)

def testing_std(data) -> float:
    # call the sum_custom function that is taking the array of float and the size of the array

    my_lib.standard_deviation.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_float]
    my_lib.standard_deviation.restype = ctypes.c_float

    c_float_array = (ctypes.c_float*SIZE)(*data)

    result = my_lib.standard_deviation(c_float_array, SIZE, testing_avg(data))
    return float(result)

def testing_normalization(data, ema, ema_std):
    # call the sum_custom function that is taking the array of float and the size of the array
    result = np.zeros(SIZE)
    my_lib.data_normalizer.argtypes = [ctypes.POINTER(ctypes.c_int),
                                       ctypes.c_int,
                                       ctypes.POINTER(ctypes.c_float),
                                       ctypes.POINTER(ctypes.c_float),
                                       ctypes.POINTER(ctypes.c_float)]
    # my_lib.data_normalizer.restype = ctypes.c_float

    c_int_array = (ctypes.c_int*SIZE)(*data)
    c_float_array = (ctypes.c_float*SIZE)(*ema)
    c_float_array_std = (ctypes.c_float*SIZE)(*ema_std)
    c_float_array_result = (ctypes.c_float*SIZE)(*result)


    my_lib.data_normalizer(c_int_array, SIZE, c_float_array, c_float_array_std, c_float_array_result)
    return list(c_float_array_result)


import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='IOP cpp methods test')
    parser.add_argument('--full-run',
                        help='run the while loop',
                        action='store_true')
    if not parser.parse_args().full_run:
        # print('Buffer : ', len(BUFFER))
        ema = np.zeros(SIZE)
        ema_std = np.zeros(SIZE)
        counter = 0
        sleep = False
        while True :
            random_data = np.random.randint(0,300, size= SIZE) # Representing the 200 freq from 100khz to 300khz
            # print('random data : ', random_data)
            ema = testing_ema(random_data, ema)
            ema_std = testing_ema_std(random_data, ema_std, ema)
            # print('ema : ', ema)

            buffer[counter] = random_data
            # avg = np.mean(buffer, axis=0)
            std = np.std(buffer, axis=0)
            # print('np avg : ', avg)
            print('np std : ', std)

            diff = abs(ema_std - std)
            
            print('diff : ', diff)

            if counter < BUFFER_SIZE-1:
                counter += 1
            else:
                counter = 0
                sleep = True
            
            if sleep:
                time.sleep(0.1)


    else:
        print('full run')
        ema = np.zeros(SIZE)
        ema_std = np.zeros(SIZE)
        try:
            counter = 0
            old_sum_normalized = 0
            while True:
                if counter % 100:
                    # print(f'counter {counter}')
                    data = np.random.randint(800,1024, size= SIZE) # Representing the 200 freq from 100khz to 300khz
                else:
                    data = np.random.randint(0,300, size= SIZE) # Representing the 200 freq from 100khz to 300khz
                ema = testing_ema(data, ema)
                ema_std = testing_ema_std(data, ema_std, ema)
                normalized_data = testing_normalization(data, ema, ema_std)
                
                avg = testing_avg(normalized_data)
                std = testing_std(normalized_data)
                
                sum_swept_data = testing_sum(normalized_data)
                diff = abs(sum_swept_data - old_sum_normalized)

                threshed = diff > 90.0
                if threshed:
                    
                    print(f'avg {avg} std {std} sum {sum_swept_data} diff {diff} threshed {threshed}')
                
                old_sum_normalized = sum_swept_data
                counter += 1
                pass
        
        except KeyboardInterrupt:
            print("bye")