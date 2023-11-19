import socket
import threading

server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

HOST = 'localhost'
PORT = 5050
server_address = (HOST,PORT)
server.bind(server_address)

BACKLOGS = 5
server.listen(BACKLOGS)
print("Server is listening at ", server_address)

BUFFER_SIZE = 1024

def handle_client(client_socket,client_address):
    print("Connected to ", client_address)
    while (True):
        msg = client_socket.recv(BUFFER_SIZE).decode()
        print("Received message : ", msg)
        client_socket.sendall(msg.encode())
        if(msg == "Bye"):
            print("Disconnected from ", client_address)
            client_socket.close()
            break

while True:
    client_socket,client_address = server.accept()
    thread = threading.Thread(target = handle_client, args=(client_socket,client_address))
    thread.start()
