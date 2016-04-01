# This shows an example of using the publish.single helper function.

import sys
import paho.mqtt.publish as publish
from paho.mqtt.publish import *

#publish.single("led/blink/led1", 'led45645', hostname="192.168.43.252")
#publish.single("hello/world2", "fade-1-200", hostname="172.16.0.2")
#publish.single("led/switch/led2", 'ledop', hostname="192.168.43.97")
publish.single("testtopic/roll", payload="rotit", qos=2, retain=False, hostname="broker.mqttdashboard.com",port=8000, client_id="clientId-GeOIJCXuvV", keepalive=60, will=None, auth=None, tls=None,protocol=mqtt.MQTTv31)

