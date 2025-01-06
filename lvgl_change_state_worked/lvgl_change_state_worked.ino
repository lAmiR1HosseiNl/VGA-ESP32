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
const char *ssid = "AmirHossein";
const char *password = "amir1382raeghi";

const char *mqtt_broker = "broker.emqx.io";
const char *topic = "Employees Es4031/topic";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

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
            delay(2000);
        }
    }
}

/****************************************************
 * 6) Screen Management Functions
 ****************************************************/
lv_obj_t *idle_screen;
lv_obj_t *success_screen;
lv_obj_t *failure_screen;
lv_obj_t *table;

unsigned long last_screen_change = 0;
const unsigned long screen_timeout = 3000;

const int max_entries = 5;
struct Attendee {
    String name;
    String enter_time;
    String exit_time;
};
Attendee attendees[max_entries];

void update_table() {
    lv_table_set_row_cnt(table, max_entries + 1); // Extra row for header

    // Set headers
    lv_table_set_cell_value(table, 0, 0, "Name");
    lv_table_set_cell_value(table, 0, 1, "Enter Time");
    lv_table_set_cell_value(table, 0, 2, "Exit Time");

    for (int i = 0; i < max_entries; i++) {
        lv_table_set_cell_value(table, i + 1, 0, attendees[i].name.c_str());
        lv_table_set_cell_value(table, i + 1, 1, attendees[i].enter_time.c_str());
        lv_table_set_cell_value(table, i + 1, 2, attendees[i].exit_time.c_str());
    }
}

void create_screens() {
    // Idle Screen with Table
    idle_screen = lv_obj_create(NULL);
    table = lv_table_create(idle_screen);
    lv_obj_set_size(table, 300, 150);
    lv_obj_align(table, LV_ALIGN_CENTER, 0, 0);
    update_table();

    // Success Screen
    success_screen = lv_obj_create(NULL);
    lv_obj_t *success_label = lv_label_create(success_screen);
    lv_label_set_text(success_label, "Login Successful!");
    lv_obj_align(success_label, LV_ALIGN_CENTER, 0, 0);

    // Failure Screen (With Red Background)
    failure_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(failure_screen, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_t *failure_label = lv_label_create(failure_screen);
    lv_label_set_text(failure_label, "Login Failed!");
    lv_obj_align(failure_label, LV_ALIGN_CENTER, 0, 0);

    lv_scr_load(idle_screen);
}

void switch_screen(lv_obj_t *screen) {
    lv_scr_load(screen);
    lv_refr_now(NULL);
    last_screen_change = millis();
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
        message += (char)payload[i];
        Serial.print((char)payload[i]);
    }
    Serial.println("\n-----------------------");

    message.trim();

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.print("JSON Parsing Error: ");
        Serial.println(error.c_str());
        return;
    }

    String status = doc["status"] | "Unknown";
    String name = doc["name"] | "Unknown";
    String enter_time = doc["enter_time"] | "N/A";
    String exit_time = doc["exit_time"] | "N/A";

    Serial.printf("Parsed: Name: %s, Enter Time: %s, Exit Time: %s, Status: %s\n",
                  name.c_str(), enter_time.c_str(), exit_time.c_str(), status.c_str());

    if (status == "Success") {
        switch_screen(success_screen);
    } else if (status == "Failed") {
        switch_screen(failure_screen);
    }

    // Shift previous entries up
    for (int i = max_entries - 1; i > 0; i--) {
        attendees[i] = attendees[i - 1];
    }

    // Add new entry at index 0
    attendees[0] = {name, enter_time, exit_time};

    // Update table in idle screen
    update_table();
}

/****************************************************
 * 8) Setup and Loop
 ****************************************************/
void setup() {
    Serial.begin(115200);
    vga.init(VGAMode::MODE320x240, VGA_RED_PIN, VGA_GREEN_PIN, VGA_BLUE_PIN, VGA_HSYNC_PIN, VGA_VSYNC_PIN);
    vga.clear();

    connectToWiFi();
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    connectToMQTT();

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = lvgl_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    create_screens();
}

void loop() {
    if (!client.connected()) connectToMQTT();
    client.loop();

    if (millis() - last_screen_change > screen_timeout) {
        lv_scr_load(idle_screen);
        lv_refr_now(NULL);
    }

    lv_timer_handler();
    delay(5);
}
