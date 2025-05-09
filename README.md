# 🌤️ ESP8266 Real-Time Weather & Clock Display

This project uses a **NodeMCU ESP8266** microcontroller to fetch and display **real-time weather data** and a **digital clock** on a **1.3" SH1106 OLED display**. It retrieves weather information via the **OpenWeatherMap API** and synchronizes time using **NTP (Network Time Protocol)**.

---

## 📌 Features

- 📡 Connects to WiFi to fetch live data
- 🌍 Displays weather for any city (configurable)
- ⏰ Real-time clock synced with NTP servers (adjusted for IST)
- 🌡️ Shows temperature, humidity, rain chance, and weather description
- 🖥️ OLED display with clean layout and blinking colon effect
- 🔄 Weather data auto-refreshes every 60 seconds

---

## 🧰 Hardware Requirements

- [x] NodeMCU ESP8266 (e.g. ESP-12E)
- [x] 1.3" SH1106 OLED display (I2C)
- [x] Micro-USB cable
- [x] Internet WiFi connection

---

## 🔧 Libraries Used

Ensure these libraries are installed in your Arduino IDE:

- `ESP8266WiFi.h`
- `WiFiClient.h`
- `Wire.h`
- `U8g2lib.h`
- `NTPClient.h`
- `WiFiUdp.h`
- `ArduinoJson.h`

---

## 🔌 Wiring (I2C)

| OLED Pin | NodeMCU Pin |
|----------|-------------|
| VCC      | 3V3         |
| GND      | GND         |
| SDA      | D2 (GPIO4)  |
| SCL      | D1 (GPIO5)  |

---

## ⚙️ Setup Instructions

1. **Clone this repo** or download the `.ino` file.
2. Open it in **Arduino IDE**.
3. Update these values in the code:
   ```cpp
   const char* ssid     = "your_wifi_ssid";
   const char* password = "your_wifi_password";
   const char* apiKey   = "your_openweathermap_api_key";
   const char* city     = "YourCity";
   const char* country  = "YourCountryCode";  // e.g. "IN"
