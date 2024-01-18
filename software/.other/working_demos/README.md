# Working demo folder


## analog_serial_data demo

This demo is reading the analog data using the esp32. Then the data is sent through the serial port to the computer. The data is read by the python script and plotted in real time. The python script is sending the data using socket to a pure-data server. The pure-data server is doing the sonification in real-time.

### How to run the demo

1. Upload the analog_serial_data/src/main.cpp to the esp32
2. Configure the python script to use the correct serial port. Also specify the ip address and port of the pure-data server.
3. Run the python script :
```bash
python3 analog_serial_data/src/app.py
```
4. Run the pure-data server patch
```bash
pure-data analog_serial_data/src/receiveTCP.pd
```

### Acknowledgement

Thanks to [Nicolas Stas (@COLVERTYETY)](https://github.com/COLVERTYETY) for the help on the demo.