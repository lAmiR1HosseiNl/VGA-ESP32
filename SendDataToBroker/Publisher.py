#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32Video.h>          
#include <Ressources/Font6x8.h>

const char* ssid         = "AmirHossein";
const char* password     = "amir1382raeghi";
const char* mqtt_broker  = "broker.emqx.io";
const char* mqtt_topic   = "Employees Es4031/topic";
const int   mqtt_port    = 1883;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

const int VGA_RED_PIN   = 18;
const int VGA_GREEN_PIN = 22;
const int VGA_BLUE_PIN  = 21;
const int VGA_HSYNC_PIN = 16;
const int VGA_VSYNC_PIN = 17;

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

VGA3Bit vga;

struct Employee {
  int     id;
  String  name;
  String  enter_time;
  String  exit_time;
};

#include <vector>
std::vector<Employee> employees;

enum DeviceState {
  STATE_IDLE,
  STATE_SUCCESS,
  STATE_FAILED
};

DeviceState currentState = STATE_IDLE;
unsigned long stateStartTime = 0;

String lastSuccessName = "";

void fillScreen(uint16_t color) {
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      vga.dotFast(x, y, color);
    }
  }
}

void drawBox(int x, int y, int width, int height, uint16_t borderColor, uint16_t fillColor) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
        vga.dotFast(x + j, y + i, borderColor);
      } else {
        vga.dotFast(x + j, y + i, fillColor);
      }
    }
  }
}

void printText(int x, int y, const char* text, uint16_t color) {
  vga.setTextColor(color);
  vga.setCursor(x, y);
  vga.print(text);
}

void drawIdleScreen() {
  fillScreen(vga.RGB(255, 255, 255));
  drawBox(5, 5, 310, 230, vga.RGB(0, 0, 0), vga.RGB(148, 227, 178));
  printText(10, 10, "Employee List", vga.RGB(0, 0, 0));
  int startY = 30;
  printText(10, startY, "ID   NAME           ENTER_TIME          EXIT_TIME", vga.RGB(0, 0, 0));
  int rowSpacing = 10;
  int y = startY + rowSpacing + 5;
  for (size_t i = 0; i < employees.size(); i++) {
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "%-4d %-15s %-20s %-20s", employees[i].id, employees[i].name.substring(0,15).c_str(), employees[i].enter_time.substring(0,20).c_str(), employees[i].exit_time.substring(0,20).c_str());
    printText(10, y, buffer, vga.RGB(0, 0, 0));
    y += rowSpacing;
    if (y > SCREEN_HEIGHT - 10) break;
  }
}

void drawSuccessScreen() {
  fillScreen(vga.RGB(0, 255, 0));
  drawBox(40, 30, 240, 180, vga.RGB(0, 0, 0), vga.RGB(148, 227, 178));
  String welcomeMsg = "Welcome " + lastSuccessName;
  printText(50, 80, welcomeMsg.c_str(), vga.RGB(0, 0, 0));
  printText(140, 120, "✓", vga.RGB(0, 0, 0));
}

void drawFailedScreen() {
  fillScreen(vga.RGB(255, 0, 0));
  drawBox(40, 30, 240, 180, vga.RGB(0, 0, 0), vga.RGB(148, 227, 178));
  printText(50, 80, "Failed to login", vga.RGB(0, 0, 0));
  printText(140, 120, "✗", vga.RGB(0, 0, 0));
}

void updateDisplay() {
  switch (currentState) {
    case STATE_IDLE:
      drawIdleScreen();
      break;
    case STATE_SUCCESS:
      drawSuccessScreen();
      break;
    case STATE_FAILED:
      drawFailedScreen();
      break;
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  String message;
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
  Serial.println("Payload: " + message);
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }
  int    id           = doc["ID"]           | -1;
  String name         = doc["name"]         | "";
  String enter_time   = doc["enter_time"]   | "";
  String exit_time    = doc["exit_time"]    | "";
  String status       = doc["status"]       | "";
  Serial.printf("Parsed => ID:%d, Name:%s, Status:%s\n", id, name.c_str(), status.c_str());
  bool found = false;
  for (auto &emp : employees) {
    if (emp.id == id) {
      emp.name         = name;
      emp.enter_time   = enter_time;
      emp.exit_time    = exit_time.isEmpty() ? "N/A" : exit_time;
      found = true;
      break;
    }
  }
  if (!found) {
    Employee newEmp;
    newEmp.id           = id;
    newEmp.name         = name;
    newEmp.enter_time   = enter_time;
    newEmp.exit_time    = exit_time.isEmpty() ? "N/A" : exit_time;
    employees.push_back(newEmp);
  }
  if (status.equalsIgnoreCase("Success")) {
    currentState = STATE_SUCCESS;
    lastSuccessName = name;
    stateStartTime = millis();
  } else if (status.equalsIgnoreCase("Failed")) {
    currentState = STATE_FAILED;
    stateStartTime = millis();
  } else {
    currentState = STATE_IDLE;
  }
  updateDisplay();
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  while (!mqttClient.connected()) {
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    Serial.print("Connecting to MQTT... ");
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected.");
      mqttClient.subscribe(mqtt_topic);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic);
    } else {
      Serial.print("failed with state ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  vga.init(VGAMode::MODE320x240, VGA_RED_PIN, VGA_GREEN_PIN, VGA_BLUE_PIN, VGA_HSYNC_PIN, VGA_VSYNC_PIN);
  vga.setFont(Font6x8);
  vga.clear(vga.RGB(0, 0, 0));
  Serial.println("VGA Initialized.");
  connectToWiFi();
  mqttClient.setServer(mqtt_broker, mqtt_port);
  mqttClient.setCallback(mqttCallback);
  connectToMQTT();
  currentState = STATE_IDLE;
  updateDisplay();
}

void loop() {
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();
  if (currentState == STATE_SUCCESS || currentState == STATE_FAILED) {
    if (millis() - stateStartTime >= 5000) {
      currentState = STATE_IDLE;
      updateDisplay();
    }
  }
}
