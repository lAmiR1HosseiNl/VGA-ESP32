#include <WiFi.h>                // For ESP32
#include <PubSubClient.h>        // MQTT library

// WiFi credentials
const char* ssid = "AmirHossein";           
const char* password = "amir1382raeghi";   

// MQTT broker details
const char* mqtt_server = "broker.emqx.io";    // MQTT broker
const int mqtt_port = 1883;                    // MQTT port
const char* mqtt_topic = "test/topic";         // MQTT topic to subscribe to

WiFiClient espClient;
PubSubClient client(espClient);

// Callback function to handle incoming messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  
  Serial.print("Message: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Reconnect to MQTT broker if disconnected
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected!");
      // Subscribe to the topic
      client.subscribe(mqtt_topic);
      Serial.println("Subscribed to topic!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  setup_wifi();         // Connect to WiFi
  client.setServer(mqtt_server, mqtt_port);  // Set MQTT broker and port
  client.setCallback(callback);             // Set callback function
}

void loop() {
  if (!client.connected()) {
    reconnect();        // Reconnect if disconnected
  }
  
  client.loop();        // Process MQTT messages
}
