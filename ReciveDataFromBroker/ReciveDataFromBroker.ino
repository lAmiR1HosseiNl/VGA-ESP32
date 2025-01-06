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
#include "Arial12.h"
#include "Arial8.h"
/*******************************************************************
 * drawCharArial12():
 *   - Renders a single character C at (x, y) using the 18-pixel-high 
 *     variable-width font data from Arial12.h
 *   - color is a VGA color from vga.RGB(r,g,b).
 *******************************************************************/
void drawCharArial12(VGA3Bit &vga, int x, int y, char c, uint16_t color)
{
  // 1) Ensure the char is in range 0x20..0x7F
  if (c < 32 || c > 127) {
    c = '?'; // or just return if you prefer to skip
  }
  
  // 2) Look up the 'index' in Arial12Index_table for this character
  uint8_t index = Arial12Index_table[(uint8_t)c];
  
  // 3) Get the glyph's width and offset in the data array
  uint8_t  width  = Arial12Width_table[index];
  uint16_t offset = Arial12Offset_table[index];
  
  // 4) Figure out how many bytes total store this glyph
  //    by subtracting next offset (watch for boundary)
  uint16_t nextOffset = 0;
  if (index < 95) // (There are 96 printable chars from 0x20 to 0x7F in that table)
    nextOffset = Arial12Offset_table[index + 1];
  else
    nextOffset = sizeof(Arial12Data_table); // fallback

  uint16_t glyphSize = nextOffset - offset;

  // 5) Pointer to the raw bitmap data for this glyph
  const unsigned char *glyphData = &Arial12Data_table[offset];

  // 6) The font is stated as 18 pixels tall, so we loop rows 0..17
  //    For each row, we read 'width' bits from the glyphData in big-endian
  //    bitIndex = row * width + col
  //    byteIndex = bitIndex / 8
  //    bitInByte = 7 - (bitIndex % 8)
  for (int row = 0; row < 18; row++)
  {
    for (int col = 0; col < width; col++)
    {
      int bitIndex   = row * width + col;       // Which bit overall
      int byteIndex  = bitIndex / 8;            // Which byte in glyphData
      int bitInByte  = 7 - (bitIndex % 8);      // Big-endian
      if (byteIndex < glyphSize)
      {
        unsigned char b = glyphData[byteIndex];
        if ((b >> bitInByte) & 0x01)
        {
          // Pixel is 'on'
          vga.dotFast(x + col, y + row, color);
        }
        else
        {
          // If you want a background color, you could draw it here.
          // Otherwise, do nothing for 'off' pixel
        }
      }
    }
  }
}

/*******************************************************************
 * drawStringArial12():
 *   - Loops over each character in 'text' and calls drawCharArial12().
 *   - Moves 'x' horizontally by (glyphWidth + 1) each time.
 *   - Honor '\n' by moving down 18 pixels and resetting x to start.
 *******************************************************************/
void drawStringArial12(VGA3Bit &vga, int x, int y, const char *text, uint16_t color)
{
  int startX = x;  // so we can return to left margin on '\n'

  while (*text)
  {
    char c = *text++;

    if (c == '\n')
    {
      // Move down one line, back to startX
      x  = startX;
      y += 18; // font height
      continue;
    }

    // Render one character
    uint8_t index = Arial12Index_table[(uint8_t)c];
    uint8_t width = Arial12Width_table[index];

    drawCharArial12(vga, x, y, c, color);

    // Advance x by the character's width plus maybe 1 for spacing
    x += width + 1;
  }
}

void drawCharArial8(VGA3Bit &vga, int x, int y, char c, uint16_t color)
{
  if (c < 32 || c > 127) c = '?';

  uint8_t index = Arial8Index_table[(uint8_t)c];
  uint8_t width = Arial8Width_table[index];
  uint16_t offset = Arial8Offset_table[index];
  uint16_t nextOffset = (index < 95) ? Arial8Offset_table[index + 1] : sizeof(Arial8Data_table);
  uint16_t glyphSize = nextOffset - offset;
  const unsigned char *glyphData = &Arial8Data_table[offset];

  for (int row = 0; row < 10; row++) // Arial8 is 10px high
  {
    for (int col = 0; col < width; col++)
    {
      int bitIndex = row * width + col;
      int byteIndex = bitIndex / 8;
      int bitInByte = 7 - (bitIndex % 8);

      if (byteIndex < glyphSize)
      {
        unsigned char b = glyphData[byteIndex];
        if ((b >> bitInByte) & 0x01)
          vga.dotFast(x + col, y + row, color);
      }
    }
  }
}

void drawStringArial8(VGA3Bit &vga, int x, int y, const char *text, uint16_t color)
{
  int startX = x;
  while (*text)
  {
    char c = *text++;
    if (c == '\n') { x = startX; y += 10; continue; }

    uint8_t index = Arial8Index_table[(uint8_t)c];
    uint8_t width = Arial8Width_table[index];
    drawCharArial8(vga, x, y, c, color);
    x += width + 1;
  }
}


/****************************************************
 * 2) Wi-Fi and MQTT Credentials
 *    (Adjust for your environment)
 ****************************************************/
const char* ssid         = "AmirHossein";
const char* password     = "amir1382raeghi";

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
 * 12) Draw Box: Draws a rectangle with a border
 ****************************************************/
void drawBox(int x, int y, int width, int height, uint16_t borderColor, uint16_t fillColor)
{
  // Fill the box
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      vga.dotFast(j, i, fillColor);
    }
  }
  // Draw the border
  for (int i = 0; i < width; i++) {
    vga.dotFast(x + i, y, borderColor); // Top border
    vga.dotFast(x + i, y + height - 1, borderColor); // Bottom border
  }
  for (int i = 0; i < height; i++) {
    vga.dotFast(x, y + i, borderColor); // Left border
    vga.dotFast(x + width - 1, y + i, borderColor); // Right border
  }
}

/****************************************************
 * 13) IDLE Screen
 *     - Blue background
 *     - Shows Employee table ignoring status column
 ****************************************************/
void drawIdleScreen()
{
  // 1) Blue background
  fillScreen(vga.RGB(255, 255, 255));

  // 2) Title
  drawStringArial12(vga ,10, 10, "Employee List", vga.RGB(0, 0, 0));

  // 3) Table header with borders
  int tableX = 10;
  int tableY = 30;
  int rowHeight = 12;
  int colWidths[] = {30, 100, 80, 80};

  // Draw header background
  drawBox(tableX, tableY, /*width*/ (colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3]),
          /*height*/ rowHeight,
          /*borderColor*/ vga.RGB(0, 0, 0),
          /*fillColor*/   vga.RGB(148, 227, 178));  drawStringArial8(vga, tableX + 2, tableY + 2, "ID", vga.RGB(0, 0, 0));
  drawStringArial8(vga, tableX + colWidths[0] + 2, tableY + 2, "Name", vga.RGB(0, 0, 0));
  drawStringArial8(vga, tableX + colWidths[0] + colWidths[1] + 2, tableY + 2, "Enter Time", vga.RGB(0, 0, 0));
  drawStringArial8(vga, tableX + colWidths[0] + colWidths[1] + colWidths[2] + 2, tableY + 2, "Exit Time", vga.RGB(0, 0, 0));

  // Draw each employee row
  int y = tableY + rowHeight;
  for (size_t i = 0; i < employees.size(); i++) {
    if (y + rowHeight > SCREEN_HEIGHT - 10) break; // Prevent overflow

    // Draw row background
    drawBox(tableX, y,
            colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3],
            rowHeight,
            vga.RGB(0, 0, 0),
            vga.RGB(255, 255, 255));
    // Print row data
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d", employees[i].id);
    drawStringArial8(vga, tableX + 2, y + 2, buffer, vga.RGB(0, 0, 0));

    drawStringArial8(vga, tableX + colWidths[0] + 2, y + 2, employees[i].name.substring(0, 15).c_str(), vga.RGB(0, 0, 0));
    drawStringArial8(vga, tableX + colWidths[0] + colWidths[1] + 2, y + 2, employees[i].enter_time.c_str(), vga.RGB(0, 0, 0));
    drawStringArial8(vga, tableX + colWidths[0] + colWidths[1] + colWidths[2] + 2, y + 2, employees[i].exit_time.c_str(), vga.RGB(0, 0, 0));

    y += rowHeight;
  }
}

/****************************************************
 * 14) SUCCESS Screen
 *     - Green background
 *     - Shows "Welcome {Name}" + tick icon
 ****************************************************/
void drawSuccessScreen()
{
  fillScreen(vga.RGB(148, 227, 178)); // greenish background
  drawBox(30, 30, 260, 100, vga.RGB(0, 0, 0), vga.RGB(148, 227, 178));

  String welcomeMsg = "Welcome " + lastSuccessName;
  drawStringArial12(vga, 40, 40, welcomeMsg.c_str(), vga.RGB(0, 0, 0));

  // Show tick icon (✓)
  // printText(40, 60, "✓", vga.RGB(0, 0, 0));
}

/****************************************************
 * 15) FAILED Screen
 *     - Red background
 *     - Shows "Failed to login" + cross icon
 ****************************************************/
void drawFailedScreen()
{
  fillScreen(vga.RGB(255, 0, 0)); // red background
  drawBox(30, 30, 260, 100, vga.RGB(0, 0, 0), vga.RGB(255, 255, 255));

  drawStringArial12(vga, 40, 40, "Failed to login", vga.RGB(0, 0, 0));

  // Show cross icon (✗)
  // printText(40, 60, "✗", vga.RGB(0, 0, 0));
}

/****************************************************
 * 16) Update the Screen Based on Current State
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
 * 17) MQTT Callback: When a Message Arrives
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

  Serial.printf("Parsed => ID:%d, Name:%s, Status:%s\n", id, name.c_str(), status.c_str());

  // Update or add the employee in the vector
  bool found = false;
  for (auto &emp : employees)
  {
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
 * 18) Connect to Wi-Fi
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
 * 19) Connect to MQTT Broker
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
 * 20) Setup
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
  vga.setFont(Font6x8);    // Use the Arial12 custom font
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
 * 21) Main Loop
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
