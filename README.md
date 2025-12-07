# Circuit Diagram - RFID Robot Control System

## Visual Circuit Diagram

![Circuit Diagram](WhatsApp%20Image%202025-12-08%20at%2012.30.47%20AM.jpeg)

## Components Required
- ESP8266 (NodeMCU)
- MFRC522 RFID Reader Module
- HW-130 Dual Motor Driver (L298N-based)
- MQ-2/MQ-3 Gas/Alcohol Sensor
- 2x DC Motors
- 12V Power Supply (for motors)
- 5V Power Supply (for ESP8266 and sensors)

## Pin Connections

### ESP8266 to MFRC522 RFID Reader
```
MFRC522 Pin    →    ESP8266 Pin
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SDA (SS)       →    D8  (GPIO 15)
SCK            →    D5  (GPIO 14)
MOSI           →    D7  (GPIO 13)
MISO           →    D6  (GPIO 12)
IRQ            →    Not Connected
GND            →    GND
RST            →    D2  (GPIO 4)
3.3V           →    3.3V
```

### ESP8266 to HW-130 Motor Driver
```
HW-130 Pin     →    ESP8266 Pin
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
IN1 (Motor A)  →    D3  (GPIO 0)
IN2 (Motor A)  →    D4  (GPIO 2)
IN3 (Motor B)  →    D1  (GPIO 5)
IN4 (Motor B)  →    D6  (GPIO 12)
ENA            →    5V (Enable Motor A - always ON)
ENB            →    5V (Enable Motor B - always ON)
VCC (Logic)    →    5V
GND            →    GND
12V            →    12V Power Supply (+)
GND            →    12V Power Supply (-)
```

### ESP8266 to Gas Sensor
```
Gas Sensor Pin →    ESP8266 Pin
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VCC            →    5V
GND            →    GND
DOUT           →    D0  (GPIO 16)
```

### HW-130 Motor Connections
```
Motor Driver   →    DC Motors
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
OUT1           →    Left Motor (+)
OUT2           →    Left Motor (-)
OUT3           →    Right Motor (+)
OUT4           →    Right Motor (-)
```

## Power Supply Connections
```
12V Supply     →    HW-130 (12V input)
5V Supply      →    ESP8266 (VIN or USB)
               →    HW-130 (VCC - logic)
               →    RFID Module (3.3V via ESP8266)
               →    Gas Sensor (VCC)

GND (Common)   →    All components share common ground
```

## ASCII Circuit Diagram
```
                    ┌─────────────────┐
                    │   12V BATTERY   │
                    └────┬────────┬───┘
                         │        │
                         │        └──────────┐
                    ┌────▼─────────────┐     │
                    │    HW-130        │     │
                    │  Motor Driver    │     │
                    │                  │     │
     ┌──────────────┤ IN1  OUT1 ──────┼─────┤ Left Motor
     │    ┌─────────┤ IN2  OUT2 ──────┘     │
     │    │    ┌────┤ IN3  OUT3 ─────────┐  │
     │    │    │  ┌─┤ IN4  OUT4 ─────────┼──┘ Right Motor
     │    │    │  │ │ ENA  12V+ ─────────┤
     │    │    │  │ │ ENB  GND  ─────────┼──┐
     │    │    │  │ │ VCC               │  │
     │    │    │  │ │ GND               │  │
     │    │    │  │ └──┬────────────────┘  │
     │    │    │  │    │                   │
     │    │    │  │    │                   │
  ┌──▼────▼────▼──▼────▼───────────────────▼──┐
  │  D3   D4   D1  D6  D2     GND      3.3V    │
  │                                             │
  │           ESP8266 NodeMCU                   │
  │                                             │
  │  D8   D5   D7  D6  D0     VIN       5V     │
  └──┬────┬────┬───┬───┬──────┬─────────┬──────┘
     │    │    │   │   │      │         │
     │    │    │   │   │      │         └───┐
     │    │    │   │   │      │             │
  ┌──▼────▼────▼───▼───┘   ┌──▼──────────┐  │
  │  SDA SCK MOSI MISO     │ Gas Sensor   │  │
  │                        │              │  │
  │   MFRC522 RFID         │ VCC  ────────┼──┘
  │                        │ GND  ────────┼──┐
  │   RST ──── (to D2)     │ DOUT ──(D0)  │  │
  │   3.3V ─── (to 3.3V)   └──────────────┘  │
  │   GND ──── (to GND)                       │
  └───────────────────────────────────────────┘
                                         │
                                    ┌────▼────┐
                                    │   GND   │
                                    │ (Common)│
                                    └─────────┘
```

## Important Notes

⚠️ **Power Considerations:**
- ESP8266 operates at 3.3V logic but can be powered via 5V on VIN pin
- RFID module requires 3.3V (provided by ESP8266)
- HW-130 requires separate 12V for motors and 5V for logic
- **All grounds must be connected together (common ground)**

⚠️ **Pin Limitations:**
- GPIO 15 (D8) must be LOW at boot - RFID SS pin is safe
- GPIO 0 (D3) and GPIO 2 (D4) must be HIGH at boot - might need pull-up resistors
- Avoid using D0 (GPIO 16) for critical timing - used for gas sensor digital output only

⚠️ **Motor Driver:**
- ENA and ENB are connected to 5V to keep motors always enabled
- Speed control is done via PWM on IN1-IN4 pins
- If you want PWM speed control, connect ENA/ENB to PWM-capable GPIO pins instead

## System Features
✅ RFID access control
✅ Breath/alcohol detection safety lock
✅ Tank-style robot control (forward, backward, left, right)
✅ Web-based control interface
✅ Real-time status monitoring

## WiFi Configuration
```cpp
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";
```

Access the control panel at: `http://[ESP8266_IP_ADDRESS]` after connecting to WiFi.



The default ip (that worked for me) is : http://10.134.152.19
