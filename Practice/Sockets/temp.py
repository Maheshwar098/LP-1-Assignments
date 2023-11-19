import socket
import threading


HOST = 'localhost'
PORT = 5050

server_address = (HOST,PORT)

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


server.bind(server_address)


print("Server listening at ", server_address)

BACKLOGS = 5
server.listen(BACKLOGS)

BUFFER_SIZE = 1024

def handle_client(client_socket,client_address):
    print("Connected to ", client_address)
    while(True):
        msg = client_socket.recv(BUFFER_SIZE).decode()
        print("Message received from ", client_address, " : ", msg)
        client_socket.sendall(msg.encode())
        if(msg == "Bye"):
            client_socket.close()
            print("Disconnected from ",client_address)
            break
threading_pool = []
while (True) :
    client_socket,client_address = server.accept()
    thread  = threading.Thread(target=handle_client, args=(client_socket,client_address))
    threading_pool.append(thread)
    thread.start()