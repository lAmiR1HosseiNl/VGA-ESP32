import paho.mqtt.client as mqtt
import json

BROKER = "broker.emqx.io"
PORT = 1883  
TOPIC = "Employees Es4031/topic"

def on_message(client, userdata, message):
    print(f"Message received on topic {message.topic}:")
    try:
        payload = json.loads(message.payload.decode())
        print(payload)
    except json.JSONDecodeError:
        print("Failed to decode JSON message!")

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(TOPIC)
        print(f"Subscribed to topic: {TOPIC}")
    else:
        print(f"Failed to connect, return code {rc}")

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(BROKER, PORT)
    client.loop_forever()
except Exception as e:
    print(f"An error occurred: {e}")
