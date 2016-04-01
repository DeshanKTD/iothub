
import sys
from variables import *
import paho.mqtt.publish as publish
from networkdiscover import make_get_device_list,make_localnet_device_list
from btooth import *
##################################################################################################
											#API Mangement#
##################################################################################################

def handle_GET_msg(msg):
	parameters = msg.split('&')
	catagory   = parameters[0].split('=')[1]
	option     = parameters[1].split('=')[1]
	device	   = parameters[2].split('=')[1]

	if(len(parameters)==4):
		if(parameters[3].split('=')[0]=='stat'):
			mode='stat'
			if(parameters[3].split('=')[1]=='0'):
				value=0
			elif(parameters[3].split('=')[1]=='1'):
				value=1


		elif(parameters[3].split('=')[0]=='level'):
			mode='level'
			level = parameters[3].split('=')[1]


#for catogory led
	if(catagory == 'led'):
		if(option=='blink'):
			if(device=='led1'):
				publish.single("arduinoIn/led", 'blink-1', hostname=MQTT_SERVER)
				return 'led1 blink'

			if(device=='led2'):
				publish.single("arduinoIn/led", 'blink-2', hostname=MQTT_SERVER)
				return 'led2 blink'
			#send data
			#receive data

		elif(option=='switch'):
			##selecet device
			if(device=='led1'):
				publish.single("arduinoIn/led", 'switch-1', hostname=MQTT_SERVER)
				return 'led1 on off'

			if(device=='led2'):
				publish.single("arduinoIn/led", 'switch-2', hostname=MQTT_SERVER)
				return 'led1 on off'
			#send data
			#receive data

		elif(option=='fade'):
			#selecet device
			if(device=='led1'):
				publish.single("arduinoIn/led", 'fade-1-'+level, hostname=MQTT_SERVER)
				return 'dimok'

			if(device=='led2'):
				publish.single("arduinoIn/led", 'fade-2-'+level, hostname=MQTT_SERVER)
				return 'dimok'
			#send data
			#receive data
#for netowrk handel
	if catagory == 'localnet':

		#search for local net devices
		if option == 'search':
			return make_get_device_list()



#for bluetooth handle
	if catagory == 'bluetooth':

		#search for bluetooth devices
		if option == 'search':
			return scan_devices()


#for motor
	if catagory == 'motor'

		#change speed
		if option == 'speed':
			if device == 'motor1':
				publish.single("arduinoIn/motor", 'speed-1-'+level, hostname=MQTT_SERVER)
				return 'dispeed'

			if device == 'motor2':
				publish.single("arduinoIn/motor", 'speed-2-'+level, hostname=MQTT_SERVER)
				return 'dispeed'

		if option == 'direction':
			if device == 'motor1':
				publish.single("arduinoIn/motor", 'direc-1', hostname=MQTT_SERVER)
				return 'dispeed'

			if device == 'motor2':
				publish.single("arduinoIn/motor", 'direc-2', hostname=MQTT_SERVER)
				return 'dispeed'

#def handle_POST_msg(msg):
