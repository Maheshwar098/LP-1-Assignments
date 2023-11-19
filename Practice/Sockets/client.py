import socket

DESTINATION_IP = "localhost"
DESTINATION_PORT = 5050

server_address = (DESTINATION_IP,DESTINATION_PORT)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client.connect(server_address)

print("Connected to ", server_address)

BUFFER_SIZE = 1024

while(True):
    msg = input("Enter message to send : ")
    client.sendall(msg.encode())
    response = client.recv(BUFFER_SIZE).decode()
    print("Response : ", response)
    if(response == "Bye"):
        client.close()
        print("Disconnected from ", server_address)
        break
