import socket


if __name__ == '__main__':
    # server waiting for client connection and listening on port 3000

    # create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # get local machine name
    host = '10.8.1.5'

    port = 3000

    # bind to the port
    server_socket.bind((host, port))

    server_socket.listen()
    client_socket, addr = server_socket.accept()

    print("Got a connection from %s" % str(addr))

    while True:

        # establish a connection

        # msg = 'Thank you for connecting' + "\r\n"
        # client_socket.send(msg.encode('ascii'))
        msg = server_socket.listen()
        print(msg)
        # client_socket.close()

