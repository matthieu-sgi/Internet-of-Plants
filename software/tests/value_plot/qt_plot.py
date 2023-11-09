from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtCore import QThread, pyqtSignal
import pyqtgraph as pg
import serial
import numpy as np

# Constants
MAX_VALUE = 3
MIN_FREQ = 100
MAX_FREQ = 300
NUM_LINES = 400  # Number of lines to display in the waterfall

class SerialThread(QThread):
    data_received = pyqtSignal(np.ndarray)

    def run(self):
        ser = serial.Serial('/dev/ttyUSB0', 115200)
        ser.readline()  # To avoid the first line potentially corrupted
        while True:
            data = ser.readline().decode('ascii')
            values = np.array([float(val) for val in data.strip().split(',') if val], dtype=np.float32)
            # print(values.shape)
            self.data_received.emit(values)

class WaterfallDisplay(QMainWindow):
    def __init__(self):
        super().__init__()
        self.init_ui()

        # Serial thread setup
        self.serial_thread = SerialThread()
        self.serial_thread.data_received.connect(self.update_display)
        self.serial_thread.start()

    def init_ui(self):
        self.imageView = pg.ImageView()
        self.setCentralWidget(self.imageView)

        # Waterfall plot setup
        self.waterfall_data = np.zeros((NUM_LINES, MAX_FREQ-MIN_FREQ), dtype=np.float32)

    def update_display(self, data):
        # Shift data up one line
        self.waterfall_data[:-1] = self.waterfall_data[1:]

        # Insert new line at the bottom
        self.waterfall_data[-1, :] = data

        # Calculate the mean and standard deviation for normalization
        mean = np.mean(self.waterfall_data, axis=0)
        std = np.std(self.waterfall_data, axis=0)

        # Normalize and apply logarithmic scaling
        image = np.log(np.square((self.waterfall_data - mean) / std) + 1)

        # Update the ImageView with the new data
        self.imageView.setImage(image.T, autoLevels=True, autoRange=False)

if __name__ == '__main__':
    app = QApplication([])
    mainWin = WaterfallDisplay()
    mainWin.show()
    app.exec_()
