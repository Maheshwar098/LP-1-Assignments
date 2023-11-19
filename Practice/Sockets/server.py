import socket 

HOST = 'localhost'
PORT = 5050
server_address = (HOST,PORT)

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind(server_address)

print("Server listening at ", server_address)

BACKLOGS = 5

server.listen(BACKLOGS)

client_socket, client_address = server.accept()

print("Connected to client : ", client_address)

BUFFER_SIZE = 1024

while(True):
    msg = client_socket.recv(BUFFER_SIZE).decode()
    print("Received Message : ", msg)
    client_socket.sendall(msg.encode())
    if(msg == "Bye"):
        client_socket.close()
        print("Disconnected From ", client_address)
        break


