from socket import *
import _thread
import sys



def handle_http(data):
	content=data.split('/?',1)
	command=content[1].split(' ',1)
	print(command[0])
	s.send(command[0].encode('ascii'))

def handler(clientsocket, clientaddr):
    print ("Accepted connection from: m")

    while 1:
        data = clientsocket.recv(1024).decode('ascii')
        if not data:
            break
        else:
            #print(data)
            handle_http(data)
            msg = "You sent me: %s" % data
            clientsocket.send(msg.encode('ascii'))
    clientsocket.close()



if __name__ == "__main__":

    #as a server connect variable
    host = 'localhost'
    port = 80
    buf = 1024

    #as a client connect varible
    s = socket(AF_INET, SOCK_STREAM) 
    hostc = '192.168.43.8'
    portc = 23
    s.connect((hostc, portc))

    addr = (host, port)

    serversocket = socket(AF_INET, SOCK_STREAM)

    serversocket.bind(addr)

    serversocket.listen(2)

    while 1:
        print ("Server is listening for connections\n")

        clientsocket, clientaddr = serversocket.accept()
        _thread.start_new_thread(handler, (clientsocket, clientaddr))

    serversocket.close()
