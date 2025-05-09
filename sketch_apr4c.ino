#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <U8g2lib.h>

// Use this for SH1106 1.3" OLED
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// WiFi & API config
const char* ssid     = "TechanicZ";
const char* password = "pppppppK";
const char* apiKey   = "4ef7980b676e7c99efd3696d076b5a67";
const char* city     = "Kolkata";
const char* country  = "IN";

// Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // IST: UTC+5:30 updated every minute

// Weather Data
WiFiClient client;
String weatherDesc = "";
float temperature = 0;
int humidity = 0;
int rainChance = 0;
bool showSecondColon = true;
unsigned long lastToggle = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);           // I2C pins for NodeMCU/ESP8266
  u8g2.begin();                 // Init display
  u8g2.setFont(u8g2_font_6x10_tf); // Compact readable font

  connectWiFi();
  timeClient.begin();
}

void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 20, "Connecting WiFi...");
  } while (u8g2.nextPage());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 20, "WiFi connected!");
  } while (u8g2.nextPage());
  delay(1000);
}

void fetchWeather() {
  String url = "/data/2.5/weather?q=" + String(city) + "," + String(country) + "&appid=" + apiKey + "&units=metric";
  const char* host = "api.openweathermap.org";

  if (client.connect(host, 80)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  } else {
    Serial.println("Connection to API failed!");
    return;
  }

  while (client.connected() || client.available()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }

  String payload = "";
  while (client.available()) {
    payload += client.readString();
  }
  client.stop();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.println("JSON parsing failed!");
    return;
  }

  temperature = doc["main"]["temp"];
  humidity = doc["main"]["humidity"];
  weatherDesc = doc["weather"][0]["description"].as<String>();
  rainChance = doc["clouds"]["all"] ? doc["clouds"]["all"] : 0;
}

void showData() {
  timeClient.update();
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  String ampm = "AM";
  if (hours >= 12) {
    ampm = "PM";
    if (hours > 12) hours -= 12;
  }
  if (hours == 0) hours = 12;

  char timeStr[12];
  if (showSecondColon) {
    sprintf(timeStr, "%02d:%02d:%02d %s", hours, minutes, seconds, ampm.c_str());
  } else {
    sprintf(timeStr, "%02d %02d %02d %s", hours, minutes, seconds, ampm.c_str());
  }

  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 10);  u8g2.print("Kolkata: ");  u8g2.print(timeStr);
    u8g2.setCursor(0, 22);  u8g2.print("Temp: "); u8g2.print(temperature); u8g2.print(" C");
    u8g2.setCursor(0, 32);  u8g2.print("Hum : "); u8g2.print(humidity); u8g2.print(" %");
    u8g2.setCursor(0, 42);  u8g2.print("Rain: "); u8g2.print(rainChance); u8g2.print(" %");
    u8g2.setCursor(0, 54);  u8g2.print("Wthr: "); u8g2.print(weatherDesc);
  } while (u8g2.nextPage());

  Serial.println("==== WEATHER ====");
  Serial.println(timeStr);
  Serial.println("Temp: " + String(temperature) + " C");
  Serial.println("Humi: " + String(humidity) + " %");
  Serial.println("Rain: " + String(rainChance) + " %");
  Serial.println("Desc: " + weatherDesc);
  Serial.println("=================");
}

void loop() {
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();

  if (now - lastUpdate > 60000 || lastUpdate == 0) {  // every 60 seconds
    fetchWeather();
    lastUpdate = now;
  }

  if (now - lastToggle > 1000) {
    showSecondColon = !showSecondColon; // blink effect every second
    lastToggle = now;
  }

  showData();
}
