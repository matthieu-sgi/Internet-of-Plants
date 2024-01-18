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
│   ├── patches/ # Contains the Pure Data patches
│   ├── requirements.txt # Contains the python dependencies
│   ├── main.py # Contains the main python script
│   └── test/ # Contains the tests│
└── working_demos # Contains the working demos
    ├── analog_serial_data/ # Contains the demo that sends analog data through serial and send it ovr socket
    └── README.md

```
