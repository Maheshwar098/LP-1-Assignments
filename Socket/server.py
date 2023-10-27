import socket
import threading


# DEFINE IP, PORT, ETC
HEADER = 64
PORT = 5050
SERVER = socket.gethostbyname(socket.gethostname())
ADDRESS = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MSG = '!DISCONNECT'


# CREATE SERVER AND BIND IT TO A IP & PORT
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDRESS)


# FUNCTION TO HANDLE INDIVIDUAL CLIENT LOGIN AFTER START(), WILL RUN CONCURRENTLY FOR EACH CLIENT
def handle_client(conn,addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    connected = True
    while connected:
        msg_leng = conn.recv(HEADER).decode(FORMAT)
        if msg_leng:
            msg_leng = int(msg_leng)
            msg = conn.recv(msg_leng).decode(FORMAT)
            if msg == DISCONNECT_MSG:
                connected = False
            print(f"[{addr}] {msg}")
    conn.close()


def start():
    # START LISTENING ON BINDED IP & PORT
    server.listen()
    print(f"[LISTENING] server is listening on {SERVER}")
    while True:
        # CONN IS SOCKET OBJ USED TO COMMUNICATE BACK TO CLIENT, ADDR IS INFO OF CONNECTION
        conn, addr = server.accept()
        # CREATING THREADS FOR EVERY CONNECTED CLIENT, SO THAT OTHER CLIENTS NOT NEED TO WAIT
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        # PRINTS ACTIVE CONNECTIONS
        print(f"[ACTIVE CONNECTIONS] {threading.active_count()-1}.")


print(f"[STARTING] server starting ...")
start()