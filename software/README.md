# Software and Firmware

## Project Structure

The project is structured as follows:

```
.
├── README.md
├── firmware # Contains the firmware for the ESP32
│   ├── src/ # Contains the source code
│   ├── lib/ # Contains the libraries
│   ├── platformio.ini # Contains the platformio configuration
│   └── test/ # Contains the tests
├── server # Contains the software for the server
│   ├── modules/ # Contains the python modules
│   │   ├── IOPModule # compute the data
│   │   ├── IOPReceiver # receive the data
│   │   ├── IOPSender # send the data
│   │   └── IOPServer # make the brigde betwen compute, receiver and sender
│   ├── patches/ # Contains the Pure Data patches
│   ├── requirements.txt # Contains the python dependencies
│   ├── main.py # Contains the main python script
│   ├── install.sh # Install the python dependencies, the systemd service and the patches
│   ├── start.sh # Start the server
│   └── test/ # Contains the tests│
└── .other # other stuff
    ├── tests/ # Old tests
    └── working_demos/ # Old working demos


```
