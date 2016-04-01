#all things about netwrok discovery,
#connect to various interfaces are in here
import subprocess
from sys import argv
import time
from netdisco.discovery import NetworkDiscovery

#server act as a client to server in device
def connect_to_device_port(ip,port):
	s = socket(AF_INET, SOCK_STREAM)
	s.connect((ip,port))
	return s

############################################local net#########################################################################

def make_get_device_list():
	dic = make_localnet_device_list()
	iplist=""
	for ip in dic.keys():
		mac = dic[ip]
		iplist = ip+" "+mac +"<br>"

	return iplist

#make the dictionary with current diveces
def make_localnet_device_list():
	#define divices
	#save data to a file
	print 'Opening the file...'
	target_file = open('devices_localnet.txt', 'w')
	target_file.seek(0)
	target_file.truncate()

	print "writing to file...."
	p = subprocess.Popen('arp-scan --interface wlan0 --localnet', shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	for line in p.stdout.readlines():
		#print line,
		target_file.write(line)
	retval = p.wait()
	target_file.close()

	#print devices
	return read_device_file()


#this reads the created file	
def read_device_file():
	device_dic = {}
	print 'reading file ......'
	with open('devices_localnet.txt','r') as f:
		for line in f:
			#print line,
			content = line.split('\t')
			#print len(content)
			if len(content)==3:
				if 'Unknown' not in content[2]:
					print content[1] + " " + content[2]
					device_dic[content[0]]=content[2] + '@' + content[1]
	return device_dic


#print(make_get_device_list())
#############################################bluetooth discovery################################################################
#def make_bt_divice_list():
	#define diveces

#def add_device():




#def remove_device():




#def get_device_by_id()

###########################################UPNP#################################################################################

def search_upnp_services():
	netdis = NetworkDiscovery()

	netdis.scan()

	for dev in netdis.discover():
	    print(dev, netdis.get_info(dev))

	netdis.stop()
