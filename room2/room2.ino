// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

//oled AND qr
#define OLEDDISPLAY
#include <qrcode.h>
#include <SSD1306.h>

SSD1306  display(0x3c, 21, 22);
QRcode qrcode (&display);

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

String ip;

// Set LED GPIO
const int ledPin = 15;
const int ledPin2 = 17;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// Replaces placeholder with LED state value
String processor(const String& var) {
  Serial.println(var);
  if (var == "STATE") {
    if (digitalRead(ledPin)) {
      ledState = "ON";
    }
    else {
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  

  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  ip = WiFi.localIP().toString();
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO to HIGH
  server.on("/on1", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin, HIGH);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to LOW
  server.on("/off1", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin,LOW);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to HIGH
  server.on("/on2", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin2, HIGH);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to LOW
  server.on("/off2", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin2, LOW);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Start server
  server.begin();

  display.init();
  display.clear();
  display.display();


  // enable debug qrcode
  // qrcode.debug();

  // Initialize QRcode display using library
  qrcode.init();
  // create qrcode
  qrcode.create(ip);
}

void loop() {

}
