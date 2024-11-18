import datetime
import paho.mqtt.client as mqtt
import json
import time

BROKER = "broker.emqx.io"
PORT = 1883  
TOPIC = "Employees Es4031/topic"

current_time = datetime.datetime.now()
current_time = current_time.strftime("%Y-%m-%d %H:%M:%S")
id = 0
payload = {
    "ID": id,
    "name": "Unknown",
    "time": current_time,
    "status": "Failed",
}

MESSAGE = json.dumps(payload)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.publish(TOPIC, MESSAGE)
        print(f"Published JSON message: {MESSAGE}")
    else:
        print(f"Failed to connect, return code {rc}")

client = mqtt.Client()

client.on_connect = on_connect

try:
    for i in range(2):
        client.connect(BROKER, PORT)
        client.loop_start()
        time.sleep(5)
        current_time = datetime.datetime.now()
        current_time = current_time.strftime("%Y-%m-%d %H:%M:%S")
        id += 1
        payload = {
        "ID": id,
        "name": "Ali Sh",
        "time": current_time,
        "status": "Success",
        }
        MESSAGE = json.dumps(payload)
        
except Exception as e:
    print(f"An error occurred: {e}")

input("Press Enter to exit...\n")
client.loop_stop()
client.disconnect()
