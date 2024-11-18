import paho.mqtt.client as mqtt

# MQTT broker details
BROKER = "broker.emqx.io"
PORT = 1883  # Using TCP port
TOPIC = "test/topic"  # Replace with the topic you want to subscribe to

# Define callback for when the client receives a message
def on_message(client, userdata, msg):
    print(f"Received message: '{msg.payload.decode()}' on topic '{msg.topic}' with QoS {msg.qos}")

# Define callback for successful connection
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        # Subscribe to the desired topic after connection
        client.subscribe(TOPIC)
        print(f"Subscribed to topic: {TOPIC}")
    else:
        print(f"Failed to connect, return code {rc}")

# Initialize MQTT client
client = mqtt.Client()

# Assign callback functions
client.on_connect = on_connect
client.on_message = on_message

try:
    # Connect to the broker
    client.connect(BROKER, PORT)
    # Start the MQTT client loop to process network traffic
    client.loop_forever()
except Exception as e:
    print(f"An error occurred: {e}")
