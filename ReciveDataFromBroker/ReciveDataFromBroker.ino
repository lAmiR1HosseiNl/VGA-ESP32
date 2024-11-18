#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char *ssid = "AmirHossein";           // Replace with your Wi-Fi name
const char *password = "amir1382raeghi";   // Replace with your Wi-Fi password

// MQTT broker credentials
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "test/topic";          // MQTT topic to publish and subscribe to
const char *mqtt_username = "emqx";       // Optional username (for public broker, "emqx" is fine)
const char *mqtt_password = "public";     // Optional password
const int mqtt_port = 1883;               // Non-SSL port for MQTT

WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to Wi-Fi
void connectToWiFi() {
    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// Function to connect to the MQTT broker
void connectToMQTT() {
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT broker with client ID: %s\n", client_id.c_str());

        // Attempt to connect to MQTT broker
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            // Subscribe to the topic
            client.subscribe(topic);
            Serial.printf("Subscribed to topic: %s\n", topic);
        } else {
            Serial.print("Failed to connect to MQTT broker. State: ");
            Serial.println(client.state());
            delay(2000); // Retry every 2 seconds
        }
    }
}

// Callback function to handle incoming messages
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.println("-----------------------");
    Serial.printf("Message arrived in topic: %s\n", topic);
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println("\n-----------------------");
}

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    connectToWiFi();

    // Configure MQTT client
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);

    // Connect to MQTT broker
    connectToMQTT();

    // Publish a welcome message
    client.publish(topic, "Hi, I'm ESP32 ^^");
}

void loop() {
    // Ensure the client stays connected
    if (!client.connected()) {
        connectToMQTT();
    }

    // Process MQTT messages
    client.loop();
}
