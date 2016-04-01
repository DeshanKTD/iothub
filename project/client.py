# client.py  
import socket
import sys
# create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

# get local machine name
host = '192.168.2.6'
port = 3000

# connection to hostname on the port.
s.connect((host, port))                               

# Receive no more than 1024 bytes
tm = s.recv(4096)     
print("The time got from the server is %s" % tm.decode('ascii'))

while True:
	insend = input()
	s.send(insend.encode('ascii'))                                
	if insend=='quit':
		s.close()
		sys.exit()


