# This shows an example of using the publish.single helper function.

import sys
import paho.mqtt.publish as publish


def publish_new(topic,payload,ip):
	publish.single(topic, payload, hostname=ip)



#publish.single("led/blink/led1", 'led45645', hostname="192.168.43.252")
#publish.single("led/switch/led2", 'ledop', hostname="192.168.43.97")

#publish_new("arduinoIn/led","switch-1","192.168.43.97")


