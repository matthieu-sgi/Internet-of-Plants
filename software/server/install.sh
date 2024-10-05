#!/bin/bash
# check if the script is run as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run this script as root."
    exit
fi

# check if pure data is installed
if ! command -v pd >/dev/null 2>&1; then
    echo "Please install Pure Data first."
    exit
fi

# install the requirements.txt
pip3 install -r requirements.txt


# Get the current directory of the script
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Update the start script with the current script directory
sed "s|<path>|$SCRIPT_DIR|" "$SCRIPT_DIR/start.sh.tmpl" > start.sh 

# Define the systemd service file path
SERVICE_FILE_TEMPLATE="$SCRIPT_DIR/iop_server.service.tmpl"
SERVICE_FILE="$SCRIPT_DIR/iop_server.service"
# Check if the service file exists
if [ -f "$SERVICE_FILE_TEMPLATE" ]; then
    # Backup the original service file (if it exists)
    [ -f "/etc/systemd/system/iop_server.service" ] && cp "/etc/systemd/system/iop_server.service" "/etc/systemd/system/iop_server.service.bak"

    # Update the ExecStart line in the service file with the current script directory
    sed "s|ExecStart=.*|ExecStart=$SCRIPT_DIR/start.sh|" "$SERVICE_FILE_TEMPLATE" > "$SERVICE_FILE"

    # Moove the updated service file to the systemd directory
    mv "$SERVICE_FILE" /etc/systemd/system/

    # Reload the systemd daemon to apply changes
    systemctl daemon-reload

    # Enable and start the service
    systemctl enable iop_server
    systemctl start iop_server

    echo "Service installed, enabled, and started successfully."
else
    echo "Error: Service file not found at $SERVICE_FILE."
fi
