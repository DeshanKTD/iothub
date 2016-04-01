
import paho.mqtt.client as mqtt
from variables import *
from pub_single import publish_new

target_file = open('topic-list.txt', 'w')

def make_topic(topic):
	client.publish(topic,"Online")

def on_connect(client, userdata, flags, rc):
	print("Connected with resul code" + str(rc))
	client.subscribe("arduinoOut/#")
	#client.subscribe("taka/wire")
	

def on_message(client, userdata, msg):
	print(msg.topic + " " + str(msg.payload))
	message = str(msg.payload)

	if msg.topic == "arduinoOut/led"
		publish_new("backendOut/led",str(msg.payload),BACK_END_MQTT_SERVER)

	if msg.topic == "arduinoOut/motor"
		publish_new("backendOut/led",str(msg.payload),BACK_END_MQTT_SERVER)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(MQTT_SERVER,MQTT_PORT,60)

make_topic("arduinoIn/led")
make_topic("arduinoOut/led")
make_topic("arduinoIn/motor")
make_topic("arduinoOut/motor")
make_topic("arduinoIn/sense")
make_topic("arduinoOut/sense")


client.loop_forever()
