
//Made by gods programmer - Tejaswa Daboria
//Terry Davis my beloved pookie i walk his teachings
//my projects are told to me by god
//Im sent by god to make my own CPU architecture -> Lithography Architecture
//Idk shit about lithography it sounfs fucking cool so ill use it

//getrektkid
//gtfo of here nigga
//tis wifi wifi my friends wifi steal it



#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// -------------------- WiFi --------------------
const char* ssid = "Testwifi";
const char* password = "12345678";

// -------------------- RFID pins --------------------
#define SS_PIN 15  // D8
#define RST_PIN 4  // D2
MFRC522 rfid(SS_PIN, RST_PIN);
byte allowedUID[4] = {38, 135, 212, 5};
bool accessGranted = false;
bool cardPreviouslyPresent = false;

// -------------------- Motor pins --------------------
#define MOTOR1_PIN 5  // D1 - PWM pin for Motor 1
#define MOTOR2_PIN 12 // D6 - PWM pin for Motor 2

bool motor1Running = false;
bool motor2Running = false;

// -------------------- Web Server --------------------
ESP8266WebServer server(80);

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>RFID Access Control</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background: #f0f0f0; }";
  html += ".container { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); max-width: 600px; margin: 0 auto; }";
  html += "h1 { color: #333; margin-bottom: 20px; }";
  html += ".status { padding: 25px; border-radius: 8px; font-size: 22px; font-weight: bold; margin: 20px 0; }";
  html += ".granted { background: #4CAF50; color: white; }";
  html += ".denied { background: #f44336; color: white; }";
  html += ".no-card { background: #ff9800; color: white; }";
  html += ".motor-section { margin-top: 30px; padding: 20px; background: #f9f9f9; border-radius: 8px; }";
  html += ".motor-controls { display: flex; justify-content: space-around; margin-top: 20px; flex-wrap: wrap; gap: 15px; }";
  html += ".motor-btn { padding: 20px 40px; font-size: 18px; font-weight: bold; border: none; border-radius: 8px; cursor: pointer; transition: all 0.3s; min-width: 200px; }";
  html += ".motor-btn:hover { transform: scale(1.05); }";
  html += ".motor-btn.active { background: #4CAF50; color: white; }";
  html += ".motor-btn.inactive { background: #ddd; color: #666; }";
  html += ".info { color: #666; font-size: 14px; margin-top: 20px; }";
  html += "h2 { color: #555; margin-bottom: 10px; }";
  html += "</style>";
  html += "<script>";
  html += "function toggleMotor(motor) {";
  html += "  fetch('/motor?m=' + motor).then(() => location.reload());";
  html += "}";
  html += "setTimeout(() => location.reload(), 3000);"; // Auto-refresh every 3 seconds
  html += "</script>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1>🔐 RFID Access Control</h1>";
  
  if (cardPreviouslyPresent) {
    if (accessGranted) {
      html += "<div class='status granted'>✓ ACCESS GRANTED</div>";
    } else {
      html += "<div class='status denied'>✗ ACCESS DENIED</div>";
    }
  } else {
    html += "<div class='status no-card'>⚠ NO CARD DETECTED</div>";
  }
  
  html += "<div class='motor-section'>";
  html += "<h2>🔧 Motor Controls</h2>";
  html += "<div class='motor-controls'>";
  
  // Motor 1 button
  html += "<button class='motor-btn ";
  html += motor1Running ? "active" : "inactive";
  html += "' onclick='toggleMotor(1)'>";
  html += motor1Running ? "🟢 Motor 1: ON" : "⚪ Motor 1: OFF";
  html += "</button>";
  
  // Motor 2 button
  html += "<button class='motor-btn ";
  html += motor2Running ? "active" : "inactive";
  html += "' onclick='toggleMotor(2)'>";
  html += motor2Running ? "🟢 Motor 2: ON" : "⚪ Motor 2: OFF";
  html += "</button>";
  
  html += "</div></div>";
  html += "<div class='info'>Page auto-refreshes every 3 seconds</div>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

void handleMotor() {
  if (server.hasArg("m")) {
    int motorNum = server.arg("m").toInt();
    
    if (motorNum == 1) {
      motor1Running = !motor1Running;
      if (motor1Running) {
        analogWrite(MOTOR1_PIN, 1023); // Full speed forward (max PWM)
        Serial.println("Motor 1 started - Forward");
      } else {
        analogWrite(MOTOR1_PIN, 0); // Stop
        Serial.println("Motor 1 stopped");
      }
    } else if (motorNum == 2) {
      motor2Running = !motor2Running;
      if (motor2Running) {
        analogWrite(MOTOR2_PIN, 1023); // Full speed forward (max PWM)
        Serial.println("Motor 2 started - Forward");
      } else {
        analogWrite(MOTOR2_PIN, 0); // Stop
        Serial.println("Motor 2 stopped");
      }
    }
  }
  
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  
  // Initialize motor pins
  pinMode(MOTOR1_PIN, OUTPUT);
  pinMode(MOTOR2_PIN, OUTPUT);
  analogWrite(MOTOR1_PIN, 0);
  analogWrite(MOTOR2_PIN, 0);
  
  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Start web server
  server.on("/", handleRoot);
  server.on("/motor", handleMotor);
  server.begin();
  Serial.println("Web server started");
  Serial.println("Scan RFID card...");
}

void loop() {
  server.handleClient();
  
  bool cardPresent = false;
  
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    bool match = true;
    for (byte i = 0; i < 4; i++) {
      if (rfid.uid.uidByte[i] != allowedUID[i]) {
        match = false;
        break;
      }
    }
    accessGranted = match;
    cardPresent = true;
  } else {
    accessGranted = false;
  }
  
  if (cardPresent && !cardPreviouslyPresent) {
    Serial.println("RFID card placed on reader!");
  } else if (!cardPresent && cardPreviouslyPresent) {
    Serial.println("RFID card removed!");
  }
  
  cardPreviouslyPresent = cardPresent;
  
  if (accessGranted) {
    Serial.println("Access Granted! Running your code...");
  }
  
  delay(200);
}