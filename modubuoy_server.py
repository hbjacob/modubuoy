import socket
import struct
import signal
import sys
import matplotlib.pyplot as plt

verbose = False
if "-v" in sys.argv:
    verbose = True

if "-h" in sys.argv:
    host = sys.argv[sys.argv.index("-h") + 1]
else:
    host = "192.168.137.1"

port = 5000

x = 0
x_axis = []
y_axises = [[],[]]

def exit_handler(signum, frame):
    res = input("Ctrl-C was pressed. Do you really want to stop the server? y/n (p)rint?")
    if res == 'y':
        print("Closing down server... bye")
        run = False
        conn.close()
        exit()
    elif res == 'p':
        graph_handler()

def graph_handler():
    x0 = input("from> ")
    if x0 == '' or int(x0) > len(x_axis):
        x0 = 0

    plt.clf()
    x0 = int(x0)

    for i in range(len(y_axises)):
        plt.plot(x_axis[x0:-1], y_axises[i][x0:-1])
    
    plt.savefig("graph.png")
    print("Generated new graph...")


signal.signal(signal.SIGINT, exit_handler)

run = True

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((host, port))

sock.listen(1)
conn, addr = sock.accept()
if verbose:
    print(f"Connection from: {str(addr)} at port {str(port)}")
else:
    print("Connection established")

while run:        
    data = []
    for i in range(2):
        data.append( struct.unpack('1B', conn.recv(1))[0] )
        
    y_axises[0].append(data[0])
    y_axises[1].append(data[1])
    if verbose:
        print(f"From connected module: {data}")
    x_axis.append(x)
    x += 1