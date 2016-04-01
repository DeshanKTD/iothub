import paho.mqtt.client as mqtt
from pub_single import publish_new

def make_topic(topic):
	client.publish(topic,"Online")

def on_connect(client, userdata, flags, rc):
	print("Connected with resul code" + str(rc))
	client.subscribe("backendIn/#")
	#client.subscribe("taka/wire")

def on_message(client, userdata, msg):
	print(msg.topic + " " + str(msg.payload))
	message = str(msg.payload)

	if msg.topic == "backendIn/led":
		publish_new("arduinoIn/led",message,"192.168.43.252")

	if msg.topic == "backendIn/motr":
		publish_new("arduinoIn/motor",message,"192.168.43.252")


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("192.168.43.97",1883,60)

make_topic("backendIn/led")
make_topic("backendOut/led")
make_topic("backendIn/motor")
make_topic("backendOut/motor")

client.loop_forever()
