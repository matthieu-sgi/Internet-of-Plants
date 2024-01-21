#!/bin/bash
# check if the script is run as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run this script as root."
    exit
fi


# Get the current directory of the script
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Define the systemd service file path
SERVICE_FILE="$SCRIPT_DIR/iop_server.service"

# Check if the service file exists
if [ -f "$SERVICE_FILE" ]; then
    # Backup the original service file (if it exists)
    [ -f "/etc/systemd/system/iop_server.service" ] && cp "/etc/systemd/system/iop_server.service" "/etc/systemd/system/iop_server.service.bak"

    # Update the ExecStart line in the service file with the current script directory
    sed -i "s|ExecStart=.*|ExecStart=$SCRIPT_DIR/start.sh|" "$SERVICE_FILE"

    # Copy the updated service file to the systemd directory
    cp "$SERVICE_FILE" /etc/systemd/system/

    # Reload the systemd daemon to apply changes
    systemctl daemon-reload

    # Enable and start the service
    systemctl enable iop_server
    systemctl start iop_server

    echo "Service installed, enabled, and started successfully."
else
    echo "Error: Service file not found at $SERVICE_FILE."
fi
