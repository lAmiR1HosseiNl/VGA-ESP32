import sys
import paho.mqtt.client as paho
import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)
# Define a callback for connection
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully")
    else:
        print(f"Connection failed with code {rc}")

# Define a callback for publishing
def on_publish(client, userdata, mid):
    print(f"Message published with ID: {mid}")

# Create a new client instance
client = paho.Client(protocol=paho.MQTTv311)  # Explicitly define the protocol version
client.on_connect = on_connect
client.on_publish = on_publish

# Connect to the broker
if client.connect("localhost", 1883, 60) != 0:
    print("Couldn't connect to the MQTT broker")
    sys.exit(1)

# Publish a message
client.loop_start()
result = client.publish("test_topic", "Hi, paho MQTT client works fine!", qos=0)
result.wait_for_publish()
client.loop_stop()
client.disconnect()
