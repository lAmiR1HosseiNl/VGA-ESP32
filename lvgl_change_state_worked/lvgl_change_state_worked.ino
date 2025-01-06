/****************************************************
 * 1) Include Libraries
 ****************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Bitluni's ESP32 VGA library
#include <ESP32Video.h>
#include <Ressources/Font6x8.h>
#include "Arial12.h"
#include "Arial8.h"

// LVGL Graphics Library
#include <lvgl.h>

/****************************************************
 * 2) Define VGA and LVGL Settings
 ****************************************************/
const int VGA_RED_PIN = 18;
const int VGA_GREEN_PIN = 22;
const int VGA_BLUE_PIN = 21;
const int VGA_HSYNC_PIN = 16;
const int VGA_VSYNC_PIN = 17;

const int screenWidth = 320;
const int screenHeight = 240;
VGA3Bit vga;

// LVGL Display buffer
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screenWidth * 10];
static lv_color_t buf2[screenWidth * 10];

/****************************************************
 * 3) WiFi and MQTT Settings
 ****************************************************/
const char *ssid = "AmirHossein"; // Replace with your Wi-Fi name
const char *password = "amir1382raeghi"; // Replace with your Wi-Fi password

const char *mqtt_broker = "broker.emqx.io";
const char *topic = "Employees Es4031/topic"; // MQTT topic to publish and subscribe to
const char *mqtt_username = "emqx";           // Optional username
const char *mqtt_password = "public";         // Optional password
const int mqtt_port = 1883;                   // Non-SSL port for MQTT

WiFiClient espClient;
PubSubClient client(espClient);

/****************************************************
 * 4) LVGL Display Driver Function
 ****************************************************/
void lvgl_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    for (int y = area->y1; y <= area->y2; y++) {
        for (int x = area->x1; x <= area->x2; x++) {
            vga.dot(x, y, color_p->full);
            color_p++;
        }
    }
    lv_disp_flush_ready(disp);
}

/****************************************************
 * 5) WiFi and MQTT Functions
 ****************************************************/
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

void connectToMQTT() {
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT broker with client ID: %s\n", client_id.c_str());

        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            client.subscribe(topic);
            Serial.printf("Subscribed to topic: %s\n", topic);
        } else {
            Serial.print("Failed to connect to MQTT broker. State: ");
            Serial.println(client.state());
            delay(2000); // Retry every 2 seconds
        }
    }
}

lv_obj_t *success_screen;
lv_obj_t *failure_screen;

/****************************************************
 * 6) Screen Management Functions
 ****************************************************/
void create_response_screens() {
    // Success Screen
    success_screen = lv_obj_create(NULL);
    lv_obj_t *success_label = lv_label_create(success_screen);
    lv_label_set_text(success_label, "Login Successful!");
    lv_obj_align(success_label, LV_ALIGN_CENTER, 0, 0);

    // Failure Screen
    failure_screen = lv_obj_create(NULL);
    lv_obj_t *failure_label = lv_label_create(failure_screen);
    lv_label_set_text(failure_label, "Login Failed!");
    lv_obj_align(failure_label, LV_ALIGN_CENTER, 0, 0);
}

void handleParsedMessage(const String &status, const String &name, const String &enter_time, const String &exit_time) {
    if (status == "Success") {
        Serial.println("Switching to success screen...");
        lv_scr_load(success_screen);
        lv_refr_now(NULL); // Force LVGL refresh to update the screen
    } else if (status == "Failed") {
        Serial.println("Switching to failure screen...");
        lv_scr_load(failure_screen);
        lv_refr_now(NULL);
    } else {
        Serial.println("Unhandled status: " + status);
    }
}

/****************************************************
 * 7) MQTT Callback Function
 ****************************************************/
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.println("-----------------------");
    Serial.printf("Message arrived in topic: %s\n", topic);
    Serial.print("Message: ");
    String message = "";
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        message += c;
        Serial.print(c);
    }
    Serial.println("\n-----------------------");

    message.trim(); // Remove whitespace

    Serial.println("Processed Message: " + message);

    // Parse the JSON message
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.print("JSON Parsing Error: ");
        Serial.println(error.c_str());
        return; // Exit if JSON parsing fails
    }

    // Extract fields from the JSON message
    const int id = doc["ID"] | -1; // Default to -1 if "ID" is missing
    const String name = doc["name"] | "Unknown";
    const String enter_time = doc["enter_time"] | "N/A";
    const String exit_time = doc["exit_time"] | "N/A";
    const String status = doc["status"] | "Unknown";

    Serial.printf("Parsed Message - ID: %d, Name: %s, Enter Time: %s, Exit Time: %s, Status: %s\n",
                  id, name.c_str(), enter_time.c_str(), exit_time.c_str(), status.c_str());

    // Handle the message based on the status
    handleParsedMessage(status, name, enter_time, exit_time);
}

/****************************************************
 * 8) LVGL UI Creation
 ****************************************************/
void create_ui() {
    lv_obj_t *screen = lv_scr_act();

    // Add a message to the main screen
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "Waiting for MQTT message...");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

/****************************************************
 * 9) Setup and Loop
 ****************************************************/
void setup() {
    Serial.begin(115200);

    // Initialize VGA Display
    vga.init(VGAMode::MODE320x240, VGA_RED_PIN, VGA_GREEN_PIN, VGA_BLUE_PIN, VGA_HSYNC_PIN, VGA_VSYNC_PIN);
    vga.clear();

    // Connect to WiFi
    connectToWiFi();

    // Configure and connect to MQTT
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    connectToMQTT();

    // Initialize LVGL
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * 10);

    // Set up LVGL display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = lvgl_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Create the UI
    create_ui();
    create_response_screens();
}

void loop() {
    // Ensure MQTT client stays connected
    if (!client.connected()) {
        connectToMQTT();
    }

    // Process MQTT messages
    client.loop();

    // Handle LVGL tasks
    lv_timer_handler();
    delay(5);
}
