#!/usr/bin/env python
 
from socket import *
import sys
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import os
import subprocess

def wifiscan():
	scan = subprocess.check_ooutput([sys.executable,"neighbourhood.py",34])

#Create custom HTTPRequestHandler class
class httpReqestHandler(BaseHTTPRequestHandler):
  
  #handle GET command
	def do_GET(self):
	    rootdir = 'open/' #file location
	    pin=self.path.split('/?')[1]
	    print(pin)
	    try:
	    	if not self.path.endswith('.html'):
				#f = open(rootdir + self.path) #open requested file
		 		s.send(pin.encode('ascii'))
		        #send code 200 response
		 		self.send_response(200)
		 
		        #send header first
				self.send_header('Content-type','text-html')
				self.end_headers()
		 
		        #send file content to client
				self.wfile.write('<head><title>hello</title></head><body>pin '+pin+' blink<br>takarama pissek oi</body>')
		        #f.close()
				return
	      
	    except IOError:
	    	self.send_error(404, 'file not found')
  
def run():	
	print('http server is starting...')

	#ip and port of servr
	#by default http server port is 80
	server_address = ('192.168.43.97', 80)
	httpd = HTTPServer(server_address, httpReqestHandler)
	print('http server is running...')
	httpd.serve_forever()
  
if __name__ == '__main__':
	#as a client connect varible
	s = socket(AF_INET, SOCK_STREAM) 
	hostc = '192.168.43.8'
	portc = 23
	s.connect((hostc, portc))

	run()
