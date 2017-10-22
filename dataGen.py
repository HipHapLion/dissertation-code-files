import paho.mqtt.client as mqtt
import random
import time

def genData():
    light = random.randint(100, 1200)
    tt1 = random.randint(10, 100)
    tt2 = random.randint(10, 100)
    tt3 = random.randint(10, 100)
    tt4 = random.randint(10, 100)
    tt10 = random.randint(10, 100)
    ft1 = random.randint(10, 100)
    ft3 = random.randint(10, 100)
    ft5 = random.randint(10, 100)
    now = int(time.time())
    return str(now) + "," + str(light) + "," + str(tt1) + "," + str(tt2) + "," + str(tt3) + "," + str(tt4) + "," + str(tt10) + "," + str(ft1) + "," + str(ft3) + "," + str(ft5)
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
	print("Connected with result code " + str(rc))
        # client.subscribe('sys/sensors')
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    if (msg.topic == "sys/sensors"):
        print(msg.topic + " " + str(msg.payload))
        # postData(msg.payload)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)

client.loop_start()
while True:
    client.publish("sys/sensors", genData())
    time.sleep(1) # sleep for 1 seconds before next call

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.