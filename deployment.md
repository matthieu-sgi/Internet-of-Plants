# Deployment

## Flashing the ESP32

### Requirements

- [PlatformIO](https://platformio.org/)
- [Python 3](https://www.python.org/downloads/)


### Instructions

Go to the folder `software/firmware/src/`.
Modify the 'main.cpp' file to set the correct Wifi credentials and server ip.

Then run the following command:

```bash
pio run -t upload
```


##  Run the server

### Requirements

- [Python 3](https://www.python.org/downloads/)
- [Pure Data](https://puredata.info/downloads/pure-data)

### Instructions

Go to the folder `software/server/` and run the following command:

```bash
sh install.sh
or
./install.sh
```

This script will install the required Python packages and create and enable the server.service.
