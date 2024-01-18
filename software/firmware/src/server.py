import socket

## Create a server that is listening on port 3000 and waiting for a client to connect and listen for a message






# if __name__ == '__main__':
#     # server waiting for client connection and listening on port 3000

#     # create a socket object
#     server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#     # get local machine name
#     host = '10.1.64.86'

#     port = 3000

#     # bind to the port
#     server_socket.bind((host, port))

#     server_socket.listen()
#     client_socket, addr = server_socket.accept()

#     print("Got a connection from %s" % str(addr))

#     while True:

#         # establish a connection

#         # msg = 'Thank you for connecting' + "\r\n"
#         # client_socket.send(msg.encode('ascii'))
#         msg = server_socket.listen()
#         print(msg)
#         # client_socket.close()



import socket

def handle_client(client_socket):
    while True:
        # Receive data from the client
        data = client_socket.recv(1024)
        if not data:
            break
        
        # Print the received message
        print(f"Received message from {client_socket.getpeername()}: {data.decode('utf-8')}")

    # Close the client socket when the connection is closed
    print(f"Connection from {client_socket.getpeername()} closed.")
    client_socket.close()

def start_server():
    # Set the server host and port
    host = '192.168.124.6'

    port = 13000
    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the specified host and port
    server_socket.bind((host, port))

    # Listen for incoming connections (only one client)
    server_socket.listen(1)
    print(f"Server listening on {host}:{port}")

    # Accept a client connection
    client_socket, client_address = server_socket.accept()
    print(f"Accepted connection from {client_address}")

    # Handle the single client
    handle_client(client_socket)

    # Close the server socket when done
    server_socket.close()

if __name__ == "__main__":
    start_server()
