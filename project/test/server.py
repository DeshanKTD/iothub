#!/usr/bin/python3.4
#sever.py
import socket
import time
import _thread


def handle_data(clinesocket,address):
	#print current user
	print("got connection from %s" % str(address))

	#send client current time
	currentTime=time.ctime(time.time()) + "\r\n"
	clientsocket.send(currentTime.encode('ascii'))
	
	while 1:
		get = clientsocket.recv(4096).decode('ascii')
		print(get)
		msg = "You sent me: %s" %get
		clientsocket.send(msg.encode('ascii'))
			
	clientsocket.close()



		
if __name__ == "__main__":	

	#create a  socket object
	serversocket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)

	#get local machine name

	host = 'localhost'
	port = 3000

	#bind to  the port
	serversocket.bind((host, port))

	#que up to 10 requests
	serversocket.listen(5)

	while 1:
		#listening to port
		clientsocket,address = serversocket.accept()
	
		#make a thread object for new connection and start thread	
		try:
			_thread.start_new_thread(handle_data, (clientsocket, address))
		except:
			print("unable to start the thread")
		
	serversocket.close()
	
