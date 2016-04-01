# This shows an example of using the publish.single helper function.

import sys
import paho.mqtt.publish as publish

#publish.single("led/blink/led1", 'led45645', hostname="192.168.43.252")
publish.single("backendIn/motor", 'direc-1', hostname="192.168.43.97")
#publish.single("led/switch/led2", 'ledop', hostname="192.168.43.97")


