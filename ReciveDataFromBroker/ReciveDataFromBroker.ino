/****************************************************
 * 1) Include Libraries
 ****************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Bitluni's ESP32 VGA library
#include <ESP32Video.h>          
#include <Ressources/Font6x8.h>  // Built-in 6x8 font

/****************************************************
 * 2) Wi-Fi and MQTT Credentials
 *    (Adjust for your environment)
 ****************************************************/
const char* ssid         = "Your_WiFi_SSID";
const char* password     = "Your_WiFi_Password";

const char* mqtt_broker  = "broker.emqx.io";
const char* mqtt_topic   = "Employees Es4031/topic";
const int   mqtt_port    = 1883;    // Non-SSL port

WiFiClient espClient;
PubSubClient mqttClient(espClient);

/****************************************************
 * 3) VGA Pin Configuration
 *    (Adjust as needed for your wiring)
 ****************************************************/
const int VGA_RED_PIN   = 18;
const int VGA_GREEN_PIN = 22;
const int VGA_BLUE_PIN  = 21;
const int VGA_HSYNC_PIN = 16;
const int VGA_VSYNC_PIN = 17;

/****************************************************
 * 4) Fixed Resolution to Match the VGA Mode
 *    MODE320x240 => 320 wide, 240 tall
 ****************************************************/
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

/****************************************************
 * 5) VGA Device
 ****************************************************/
VGA3Bit vga; // 3-bit mode (R,G,B each 1 bit)

/****************************************************
 * 6) Data Structure to Store Employee Info
 ****************************************************/
struct Employee {
  int     id;
  String  name;
  String  enter_time;
  String  exit_time;
  int     elapsed_time;
};

/****************************************************
 * 7) Keep a List of Employees in RAM
 ****************************************************/
#include <vector>
std::vector<Employee> employees;

/****************************************************
 * 8) Possible Device States
 ****************************************************/
enum DeviceState {
  STATE_IDLE,
  STATE_SUCCESS,
  STATE_FAILED
};

DeviceState currentState = STATE_IDLE;
unsigned long stateStartTime = 0;

/****************************************************
 * 9) Track the Last Success Name
 ****************************************************/
String lastSuccessName = "";

/****************************************************
 * 10) fillScreen(): Fills the entire screen with a color
 *     Using fixed SCREEN_WIDTH and SCREEN_HEIGHT
 ****************************************************/
void fillScreen(uint16_t color)
{
  for (int y = 0; y < SCREEN_HEIGHT; y++)
  {
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
      // draw a pixel of 'color' at (x, y)
      vga.dotFast(x, y, color);
    }
  }
}

/****************************************************
 * 11) printText(): Print text at position (x, y)
 *     with the specified color.
 ****************************************************/
void printText(int x, int y, const char* text, uint16_t color)
{
  vga.setTextColor(color);
  vga.setCursor(x, y);
  vga.print(text);
}

/****************************************************
 * 12) IDLE Screen
 *     - Blue background
 *     - Shows Employee table ignoring status column
 ****************************************************/
void drawIdleScreen()
{
  // 1) Blue background
  fillScreen(vga.RGB(0, 0, 255));

  // 2) Title
  printText(10, 10, "Employee List (IDLE State)", vga.RGB(255, 255, 255));

  // 3) Table header
  int startY = 30;
  printText(10, startY,
            "ID   NAME           ENTER_TIME          EXIT_TIME           ELAPSED",
            vga.RGB(200, 200, 200));

  // 4) Print each employee row
  int rowSpacing = 10;
  int y = startY + rowSpacing + 5;

  for (size_t i = 0; i < employees.size(); i++)
  {
    // Buffer for 1 row
    char buffer[200];
    snprintf(
      buffer,
      sizeof(buffer),
      "%-4d %-15s %-20s %-20s %-6d",
      employees[i].id,
      employees[i].name.substring(0,15).c_str(),
      employees[i].enter_time.substring(0,20).c_str(),
      employees[i].exit_time.substring(0,20).c_str(),
      employees[i].elapsed_time
    );

    printText(10, y, buffer, vga.RGB(0, 0, 0));
    y += rowSpacing;
    
    // Prevent printing past the bottom of the screen
    if (y > SCREEN_HEIGHT - 10)
      break;
  }
}

/****************************************************
 * 13) SUCCESS Screen
 *     - Green background
 *     - Shows "Welcome {Name}" + tick icon
 ****************************************************/
void drawSuccessScreen()
{
  fillScreen(vga.RGB(0, 255, 0)); // green
  String welcomeMsg = "Welcome " + lastSuccessName;
  printText(40, 40, welcomeMsg.c_str(), vga.RGB(0, 0, 0));

  // Show tick icon (✓)
  printText(40, 60, "✓", vga.RGB(0, 0, 0));
}

/****************************************************
 * 14) FAILED Screen
 *     - Red background
 *     - Shows "Failed to login" + cross icon
 ****************************************************/
void drawFailedScreen()
{
  fillScreen(vga.RGB(255, 0, 0)); // red
  printText(40, 40, "Failed to login", vga.RGB(0, 0, 0));

  // Show cross icon (✗)
  printText(40, 60, "✗", vga.RGB(0, 0, 0));
}

/****************************************************
 * 15) Update the Screen Based on Current State
 ****************************************************/
void updateDisplay()
{
  switch (currentState)
  {
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

/****************************************************
 * 16) MQTT Callback: When a Message Arrives
 ****************************************************/
void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  // Convert payload to String
  String message;
  for (unsigned int i = 0; i < length; i++)
    message += (char)payload[i];
  
  Serial.println("Payload: " + message);

  // Parse JSON
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Extract fields
  int    id           = doc["ID"]           | -1;
  String name         = doc["name"]         | "";
  String enter_time   = doc["enter_time"]   | "";
  String exit_time    = doc["exit_time"]    | "";
  String status       = doc["status"]       | "";
  int    elapsed_time = doc["elapsed_time"] | 0;

  Serial.printf("Parsed => ID:%d, Name:%s, Status:%s\n", id, name.c_str(), status.c_str());

  // Update or add the employee in the vector
  bool found = false;
  for (auto &emp : employees)
  {
    if (emp.id == id) {
      emp.name         = name;
      emp.enter_time   = enter_time;
      emp.exit_time    = exit_time.isEmpty() ? "N/A" : exit_time;
      emp.elapsed_time = elapsed_time;
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
    newEmp.elapsed_time = elapsed_time;
    employees.push_back(newEmp);
  }

  // Decide the new device state
  if (status.equalsIgnoreCase("Success"))
  {
    currentState = STATE_SUCCESS;
    lastSuccessName = name;
    stateStartTime = millis();
  }
  else if (status.equalsIgnoreCase("Failed"))
  {
    currentState = STATE_FAILED;
    stateStartTime = millis();
  }
  else
  {
    currentState = STATE_IDLE;
  }

  // Refresh display
  updateDisplay();
}

/****************************************************
 * 17) Connect to Wi-Fi
 ****************************************************/
void connectToWiFi()
{
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

/****************************************************
 * 18) Connect to MQTT Broker
 ****************************************************/
void connectToMQTT()
{
  while (!mqttClient.connected()) 
  {
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);

    Serial.print("Connecting to MQTT... ");
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected.");
      // Subscribe to the topic once connected
      mqttClient.subscribe(mqtt_topic);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic);
    } 
    else {
      Serial.print("failed with state ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

/****************************************************
 * 19) Setup
 ****************************************************/
void setup()
{
  Serial.begin(115200);
  delay(1000);

  // 1) Initialize VGA
  //    Set the mode to 320x240, and specify pins.
  //    If you are using a different mode or pins, adjust accordingly.
  vga.init(VGAMode::MODE320x240,
           VGA_RED_PIN, VGA_GREEN_PIN, VGA_BLUE_PIN,
           VGA_HSYNC_PIN, VGA_VSYNC_PIN);
  vga.setFont(Font6x8);    // Use the 6x8 built-in font
  vga.clear(vga.RGB(0, 0, 0)); // Clear with black
  Serial.println("VGA Initialized.");

  // 2) Connect to Wi-Fi
  connectToWiFi();

  // 3) Setup MQTT
  mqttClient.setServer(mqtt_broker, mqtt_port);
  mqttClient.setCallback(mqttCallback);

  // 4) Connect to MQTT
  connectToMQTT();

  // Start in IDLE
  currentState = STATE_IDLE;
  updateDisplay();
}

/****************************************************
 * 20) Main Loop
 ****************************************************/
void loop()
{
  // Keep MQTT connection alive
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();

  // If in SUCCESS or FAILED, revert to IDLE after 5 seconds
  if (currentState == STATE_SUCCESS || currentState == STATE_FAILED)
  {
    if (millis() - stateStartTime >= 5000)
    {
      currentState = STATE_IDLE;
      updateDisplay();
    }
  }
}
