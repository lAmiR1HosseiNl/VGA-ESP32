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
const char *ssid = "M.Sh";
const char *password = "13744731mnel6336";

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

const int max_entries = 10;
struct Attendee {
  int id;
  String name;
  String enter_time;
  String exit_time;
};
Attendee attendees[max_entries];
int current_entries = 0; // Track number of valid attendees

/**
 * @brief Finds the index of an attendee by their ID.
 * 
 * @param id The ID of the attendee to find.
 * @return int The index of the attendee if found; otherwise, -1.
 */
int find_attendee_index(int id) {
  for (int i = 0; i < current_entries; i++) {
    if (attendees[i].id == id) {
      return i;
    }
  }
  return -1; // Not found
}

void update_table() {
  lv_table_set_col_cnt(table, 4);                // ID, Name, Enter, Exit
  lv_table_set_row_cnt(table, max_entries + 1);  // Extra row for header

  // Set headers
  lv_table_set_cell_value(table, 0, 0, "ID");
  lv_table_set_cell_value(table, 0, 1, "Name");
  lv_table_set_cell_value(table, 0, 2, "Enter");
  lv_table_set_cell_value(table, 0, 3, "Exit");

  // Set column widths
  lv_table_set_col_width(table, 0, 40);   // ID
  lv_table_set_col_width(table, 1, 97);  // Name
  lv_table_set_col_width(table, 2, 80);   // Enter Time
  lv_table_set_col_width(table, 3, 80);   // Exit Time

  // Set a more readable font for the table
  lv_obj_set_style_text_font(table, &lv_font_montserrat_12, LV_PART_MAIN);  // Clearer font size

  for (int i = 0; i < max_entries; i++) {
    if (i < current_entries && attendees[i].id != -1) {
      String id_str = String(attendees[i].id);
      lv_table_set_cell_value(table, i + 1, 0, id_str.c_str());
      lv_table_set_cell_value(table, i + 1, 1, attendees[i].name.c_str());
      lv_table_set_cell_value(table, i + 1, 2, attendees[i].enter_time.c_str());
      lv_table_set_cell_value(table, i + 1, 3, attendees[i].exit_time.c_str());
    } else {
      lv_table_set_cell_value(table, i + 1, 0, "--"); // Placeholder for ID
      lv_table_set_cell_value(table, i + 1, 1, "--"); // Placeholder for Name
      lv_table_set_cell_value(table, i + 1, 2, "--"); // Placeholder for Enter
      lv_table_set_cell_value(table, i + 1, 3, "--"); // Placeholder for Exit
    }
  }
}

void create_screens() {
  // Idle Screen with Table
  idle_screen = lv_obj_create(NULL);
  table = lv_table_create(idle_screen);
  lv_obj_set_size(table, 300, 180);  // Reduced table size
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

void switch_screen(lv_obj_t *screen, const String &name = "") {
  if (screen == success_screen) {
    lv_obj_t *success_label = lv_obj_get_child(screen, 0);  // Get the label object
    String success_msg = "Login Successful " + name;
    lv_label_set_text(success_label, success_msg.c_str());
  }
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

  int id = doc["ID"] | -1;
  String status = doc["status"] | "Unknown";
  String name = doc["name"] | "Unknown";
  String enter_time = doc["enter_time"] | "N/A";
  String exit_time = doc["exit_time"] | "N/A";

  Serial.printf("Parsed: ID: %d, Name: %s, Enter: %s, Exit: %s, Status: %s\n",
                id, name.c_str(), enter_time.c_str(), exit_time.c_str(), status.c_str());

  if (status == "Success") {
    switch_screen(success_screen, name);
  } else if (status == "Failed") {
    switch_screen(failure_screen);
  }

  // Check if attendee already exists
  int index = find_attendee_index(id);

  if (index != -1) {
    // Attendee exists, update their exit_time
    attendees[index].exit_time = exit_time;
    Serial.printf("Updated Exit Time for ID %d: %s\n", id, exit_time.c_str());
  } else {
    // Attendee does not exist, insert as new attendee
    if (current_entries < max_entries) {
      // Shift attendees down to make room at the top
      for (int i = current_entries; i > 0; i--) {
        attendees[i] = attendees[i - 1];
      }
      // Insert the new attendee at position 0
      attendees[0] = { id, name, enter_time, exit_time };
      current_entries++;
      Serial.printf("Inserted New Attendee: ID %d\n", id);
    } else {
      // Perform a circular shift and overwrite the last attendee
      shift_attendees();
      attendees[0] = { id, name, enter_time, exit_time };
      Serial.printf("Inserted New Attendee via Circular Shift: ID %d\n", id);
    }
  }

  update_table();
}

/**************************************************** 
 * 8) Shifting Function 
 ****************************************************/
void shift_attendees() {
  if (current_entries <= 1) return; // No need to shift if there's 0 or 1 attendee

  // Store the first attendee
  Attendee first = attendees[0];

  // Shift all attendees up by one position
  for (int i = 0; i < current_entries - 1; i++) {
    attendees[i] = attendees[i + 1];
  }

  // Place the first attendee at the end of the valid entries
  attendees[current_entries - 1] = first;
}

/**************************************************** 
 * 9) Setup and Loop 
 ****************************************************/
unsigned long last_shift_time = 0;
const unsigned long shift_interval = 1000; // 1 second

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

  // Initialize all attendees to invalid state
  for (int i = 0; i < max_entries; i++) {
    attendees[i].id = -1;              // Invalid ID
    attendees[i].name = "";             // Empty name
    attendees[i].enter_time = "";       // Empty enter time
    attendees[i].exit_time = "";        // Empty exit time
  }

  current_entries = 0; // Start with zero attendees

  update_table(); // Initial table display
  last_shift_time = millis(); // Initialize shift timer
}

void loop() {
  if (!client.connected()) connectToMQTT();
  client.loop();

  // Handle screen timeout
  if (millis() - last_screen_change > screen_timeout) {
    lv_scr_load(idle_screen);
    lv_refr_now(NULL);
    last_screen_change = millis();
  }

  // Handle circular shifting every second
  if (millis() - last_shift_time >= shift_interval) {
    if (current_entries > 0) { // Only shift if there are attendees
      shift_attendees();   // Perform the circular shift
      update_table();      // Update the table display
      last_shift_time = millis(); // Reset the shift timer
    }
  }

  lv_timer_handler(); // Handle LVGL tasks
  delay(5);            // Small delay to prevent watchdog resets
}
