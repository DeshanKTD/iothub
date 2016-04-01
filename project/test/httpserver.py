

from socket import *
import sys
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import networkdiscover
from variables import *
import os
import time
import cgi
from apimgmt import handle_GET_msg

#Create custom HTTPRequestHandler class
class httpReqestHandler(BaseHTTPRequestHandler):
  
  #handle GET command
	def do_GET(self):
	    rootdir = 'open/' #file location
	    print(self.path)
	    pin=self.path.split('/?')[1]
	    print(pin)
	    try:
	    	if not self.path.endswith('.html'):
				
				time.sleep(0.5)
				#f = open(rootdir + self.path) #open requested file
		 		#s.send(pin);
				#get response back
				tm = handle_GET_msg(pin)

		        #send code 200 response
		 		self.send_response(HTTP_OK)
		 
		        #send header first
				self.send_header('Content-type','text-html')
				self.end_headers()
		 
		        #send file content to http client
				self.wfile.write('Message From the IOT HUB \n Device '+tm)
		        #f.close()
				return
	      
	    except IOError:
	    	self.send_error(HTTP_NOT_FOUND, 'file not found')


	  #handle POST command
	def do_POST(self):
	    def do_POST(self):
        # Parse the form data posted
		form = cgi.FieldStorage(fp=self.rfile, headers=self.headers,environ={'REQUEST_METHOD':'POST','CONTENT_TYPE':self.headers['Content-Type'],})

		# Begin the response
		self.send_response(200)
		self.end_headers()
		self.wfile.write('Client: %s\n' % str(self.client_address))
		self.wfile.write('User-agent: %s\n' % str(self.headers['user-agent']))
		self.wfile.write('Path: %s\n' % self.path)
		self.wfile.write('Form data:\n')

		# Echo back information about what was posted in the form
		for field in form.keys():
			field_item = form[field]
			if field_item.filename:
				# The field contains an uploaded file
				file_data = field_item.file.read()
				file_len = len(file_data)
				del file_data
				self.wfile.write('\tUploaded %s as "%s" (%d bytes)\n' % \
				(field, field_item.filename, file_len))
			else:
			# Regular form value
				self.wfile.write('\t%s=%s\n' % (field, form[field].value))
		return

  
def run():	
	print('http server is starting...')

	#ip and port of servr
	#by default http server port is 80
	server_address = (SERVER_IP , SERVER_PORT)
	httpd = HTTPServer(server_address, httpReqestHandler)
	print('http server is running...')
	httpd.serve_forever()
  
if __name__ == '__main__':
	#as a client connect varible
	run()
