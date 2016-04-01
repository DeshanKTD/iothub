#bluetooth connectivity

from bluetooth import *


def auto_connect():
	target = open('bldevice.txt','r')
	line = target.readline().split(" ")
	if line[2]=="rfcomm":
		rfcomm_connect(line[0],line[1])
		return 100
	elif line[2]=="l2cap":
		l2cap_connect(line[0],lin[1])
		return 100
	else:
		return 101



def scan_devices():
	print("performing inquiry to search bluetooth...")
	nearby_devices = discover_devices(lookup_names = True)
	#print ("found %d devices" % len(nearby_devices))
	out=""
	for name, addr in nearby_devices:
    	#print " %s - %s" % (addr, name)
		out = out + name + "@" + addr +","

	return out



def rfcomm_connect(port,addr):
	target=open('bldevice.txt','w')
	target.seek(0)
	target.truncate()
	target.write(port + " " + addr + " rfcomm")
	
	sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((bd_addr, port))
	#sock.send("hello!!")
	return sock




def l2cap_connect(port,addr):
	target=open('bldevice.txt','w')
	target.seek(0)
	target.truncate()
	target.write(port + " " + addr + " l2cap")

	sock=bluetooth.BluetoothSocket(bluetooth.L2CAP)
	sock.connect((bd_addr, port))
	#sock.send("hello!!")
	#sock.close()
	return sock




#print(scan_devices())