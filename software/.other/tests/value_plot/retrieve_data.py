import serial
import matplotlib.pyplot as plt

MAX_VALUE = 3
MIN_FREQ = 100
MAX_FREQ = 300

def get_values(nb : int = MAX_VALUE) -> list[list[str]]:
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    # To avoid the first line potentially corrupted
    ser.readline()
    ret = []
    for i in range(nb):
        data = ser.readline()
        listed_data = str(data).split(',')
        listed_data = listed_data[2:-3]
        print(listed_data)
        ret.append(listed_data)
    return ret

def data_to_csv():
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    fd = open('data.csv', 'w')
    for i in range(MAX_VALUE):
        if i == 0:
            ser.readline()
            freqs = ','.join(list(map(lambda x : f'{x}_kHz',range(MIN_FREQ, MAX_FREQ))))
            freqs += '\n'
            fd.write(freqs)
        data = ser.readline()
        str_data = str(data)
        str_data = str_data[2:-3]
        print(str_data)
        fd.write(str_data)
        fd.write('\n')
    fd.close()
    ser.close()





if __name__ == '__main__':
    # Open serial port
    # print(get_values())
