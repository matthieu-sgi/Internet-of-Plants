import typing
from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt5.QtCore import QThread, pyqtSignal
import pyqtgraph as pg
import serial
import numpy as np

# Constants
MAX_VALUE = 3
MIN_FREQ = 100
min_freq = MIN_FREQ
MAX_FREQ = 300
max_freq = MAX_FREQ
nb_value = MAX_FREQ - MIN_FREQ
# NB_VALUE = MAX_FREQ - MIN_FREQ
NUM_LINES = 200  # Number of lines to display in the waterfall

class SerialThread(QThread):
    data_received = pyqtSignal(np.ndarray)

    def run(self):
        # global min_freq, max_freq, nb_value, starting
        ser = serial.Serial('/dev/ttyUSB0', 115200)
        ser.readline()  # To avoid the first line potentially corrupted
        # Waiting for the beginning of the data
        starting = False
        
        # while not starting:
        #     data = str(ser.readline())[2:-3]
        #     print(data)
        #     # data = data.decode('ascii')
        #     if data.startswith('Beg'):
        #         data = data.strip().split(',')
        #         print(f'beginning data : {data}')
        #         min_freq = int(data[1])
        #         max_freq = int(data[2])
        #         nb_value = int(data[4])
        #         starting = True
        index = 0
        while True:
            data = ser.readline().decode('ascii')
            data = data.strip().split(',')
            if len(data) != nb_value:
                continue
            values = np.array([int((val)) for val in data if val], dtype=np.float32)
            # print(values.shape)
            # values = np.roll(values, -int((index* 5.9)))
            # index += 1
            self.data_received.emit(values)

class WaterfallDisplay(QMainWindow):
    def __init__(self):
        super().__init__()
        self.init_ui()

        # Serial thread setup
        self.serial_thread = SerialThread()
        self.serial_thread.data_received.connect(self.update_display)
        self.serial_thread.start()
        self.old_value = 0

    def init_ui(self):
        self.imageView = pg.ImageView()
        self.setCentralWidget(self.imageView)

        # Waterfall plot setup
        self.waterfall_data = np.zeros((NUM_LINES, max_freq-min_freq), dtype=np.float32)

    def update_display(self, data):
        # if starting:
        # Shift data up one line
        self.waterfall_data[:-1] = self.waterfall_data[1:]

        
        # Insert new line at the bottom
        self.waterfall_data[-1, :] = data

        # Calculate the mean and standard deviation for normalization
        mean = np.mean(self.waterfall_data, axis=0)
        std = np.std(self.waterfall_data, axis=0)
        image = np.square((self.waterfall_data - mean) / std)

        # Normalize and apply logarithmic scaling
        final_value = np.sum(image[-1])
        diff = final_value - self.old_value
        if final_value > 220 :
            print(f'final value : {final_value} | diff : {final_value - self.old_value}')
        self.old_value = final_value


        # Update the ImageView with the new data
        self.imageView.setImage(np.log(image+1), autoLevels=True, autoRange=False)

class PlotDisplay(QMainWindow):

    def __init__(self) -> None:
        super().__init__()
        self.init_ui()

        # Serial thread setup
        self.serial_thread = SerialThread()
        self.serial_thread.data_received.connect(self.update_display)
        self.serial_thread.start()


    def init_ui(self):
        self.graphWidget = pg.PlotWidget()
        self.setCentralWidget(self.graphWidget)

        # Frequency plot setup
        self.graphWidget.setYRange(0, 4096) # Voltage range
        
        self.graphWidget.setXRange(min_freq, max_freq)
        self.graphWidget.showGrid(x=True, y=True)

        # Frequency plot data
        self.freq_data = np.zeros((max_freq-min_freq), dtype=np.float32)
        self.freq_plot = self.graphWidget.plot(self.freq_data)
    
    def update_display(self, data):
        self.freq_data : np.array = data
        # Shift the values by 100 to put it in front of the real frequency
        # self.freq_data = np.roll(self.freq_data, 100)
        # Add 100 '0' to the beginning of the array to shift the values by 100
        self.freq_data = np.concatenate((np.zeros(min_freq), self.freq_data))
        
        self.freq_plot.setData(self.freq_data)




if __name__ == '__main__':
    # ser = serial.Serial('/dev/ttyUSB0', 115200)
    # ser.readline()  # To avoid the first line potentially corrupted
    # while True:
    #     data = ser.readline().decode('ascii')
    #     print(data)
        # data = data.strip().split(',')
    app = QApplication([])
    #mainWin = WaterfallDisplay()
    mainWin = PlotDisplay()
    mainWin.show()
    app.exec_()
