import paho.mqtt.client as mqtt

# MQTT broker details
BROKER = "broker.emqx.io"
PORT = 1883  # Using TCP port
TOPIC = "test/topic"  # Replace with your desired topic
MESSAGE = "Hello, MQTT!"  # Replace with your desired message

# Define callback for successful connection
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        # Publish message to topic after successful connection
        client.publish(TOPIC, MESSAGE)
        print(f"Message published: {MESSAGE}")
    else:
        print(f"Failed to connect, return code {rc}")

# Initialize MQTT client
client = mqtt.Client()

# Assign callback functions
client.on_connect = on_connect

try:
    # Connect to the broker
    client.connect(BROKER, PORT)
    # Start the MQTT client loop to process network traffic
    client.loop_start()
except Exception as e:
    print(f"An error occurred: {e}")

# Keep the script running
input("Press Enter to exit...\n")
client.loop_stop()
client.disconnect()
