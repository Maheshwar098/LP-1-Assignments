import socket

# DEFINE IP, PORT, ETC
HEADER = 64
PORT = 5050
SERVER = '192.168.22.180'
ADDRESS = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MSG = '!DISCONNECT'

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDRESS)

def send(msg):
    message = msg.encode(FORMAT)
    msg_leng = len(message)
    send_leng = str(msg_leng).encode(FORMAT)
    send_leng += b' '*(HEADER-len(send_leng))
    client.send(send_leng)
    client.send(message)
    response = client.recv(HEADER).decode(FORMAT)
    print(response)    

send("Hello World!")
