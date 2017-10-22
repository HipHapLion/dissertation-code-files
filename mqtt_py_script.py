import json
import requests
import paho.mqtt.client as mqtt

def postData(msg):
    url = 'http://localhost:3000/sensors/addData'
    arr = msg.split(",")
    payload = {
    	'date': arr[0],
    	'light': arr[1],
    	'tt1': arr[2],
    	'tt2': arr[3],
        'tt3': arr[4],
        'tt4': arr[5],
        'tt10': arr[6],
        'ft1': arr[7],
        'ft3': arr[8],
        'ft5': arr[9],
    }
    headers = {'content-type': 'application/json'}

    response = requests.post(url, data=json.dumps(payload), headers=headers)

def on_connect(client, userdata, flags, rc):
	print("Connected with result code " + str(rc))
	client.subscribe("sys/sensors")

def on_message(client, userdata, msg):
    if (msg.topic == "sys/sensors"):
        postData(msg.payload)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)
client.loop_forever()