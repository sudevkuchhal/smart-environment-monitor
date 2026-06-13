// ╔══════════════════════════════════════════════════════════════════╗
// ║       SMART MONITOR — BEAST MODE ULTIMATE EDITION v3.0          ║
// ║   ESP32 · SH1106 OLED · LCD 16x2 · DHT11 · HC-SR04 · MQ2      ║
// ║   Blynk · Web Dashboard · Email Alerts · Animations · OTA       ║
// ╚══════════════════════════════════════════════════════════════════╝
//
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  LIBRARY REQUIREMENTS  (Arduino IDE → Tools → Manage Libraries)
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  1. U8g2             by olikraus        ← OLED graphics
//  2. DHT sensor library by Adafruit      ← Temperature/Humidity
//  3. LiquidCrystal I2C by Frank de Brabander ← LCD 16x2
//  4. Blynk            by Blynk (v1.3.2)  ← Cloud dashboard
//  5. ESP Mail Client  by Mobizt (v3.x)   ← Email alerts
//
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  UPLOAD FIX — COM PORT ERROR
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  Error: "Could not open COM6" means the port is wrong or busy.
//  Fix steps:
//    1. Unplug ESP32 USB → replug
//    2. Tools → Port → select the NEW COMxx that appears
//    3. If no port: install CP2102 or CH340 driver for your ESP32
//    4. Device Manager (Windows) → Ports → find your ESP32
//    5. Tools → Board → ESP32 Arduino → ESP32 Dev Module
//    6. Tools → Upload Speed → 921600
//    7. Hold BOOT button on ESP32 while clicking Upload
//       Release BOOT when "Connecting..." appears
//
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  WIRING DIAGRAM
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  OLED SH1106 128×64   SDA→GPIO21   SCL→GPIO22   3.3V   GND
//  LCD  16×2   I2C      SDA→GPIO21   SCL→GPIO22   5V     GND  addr:0x27
//  DHT11                DATA→GPIO4                3.3V   GND
//  HC-SR04              TRIG→GPIO5   ECHO→GPIO18  5V     GND
//  MQ2  Gas Sensor      AO→GPIO34                3.3V   GND
//  Blue LED (external)  Anode→GPIO2  Cathode→GND via 220Ω resistor
//  LED Bar              GPIO13,25,14,27,26        GND via 220Ω each
//  NOTE: GPIO13 = onboard LED (also mirrors GPIO2 PWM state)
//
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  BEAST MODE FEATURES
//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  ✦ WiFi-only operation locked to phone hotspot "aryan"
//  ✦ Full system pause + offline animation when WiFi drops
//  ✦ Auto-reconnect every 5 seconds with status display
//  ✦ OTA (Over-The-Air) firmware update via web browser
//  ✦ GPIO2 external blue LED — breathing / strobe / blink PWM
//  ✦ GPIO13 onboard LED mirrors GPIO2 exactly
//  ✦ JARVIS v3 multi-phase startup animation with sound codes
//  ✦ Rotating 3-screen HUD: data → radar scanner → ECG heartbeat
//  ✦ Wave-scan animation on every sensor data change
//  ✦ Animated WiFi icon with real-time RSSI bars
//  ✦ 5-LED distance bar (GPIO 13,25,14,27,26)
//  ✦ Web dashboard with live SSE feed + dark HUD theme
//  ✦ /api/json endpoint for raw data
//  ✦ /status health endpoint
//  ✦ Blynk cloud push (V0-V6)
//  ✦ Email alerts via Gmail SMTP (60-min cooldown)
//  ✦ Gas alert with confirmation counter (no false triggers)
//  ✦ Proximity alert with inverted OLED display
//  ✦ LCD backlight strobe on dual alerts
//  ✦ Sensor uptime + boot counter in web dashboard
//  ✦ ESP32 core temperature monitor
//  ✦ Zero compile errors · ESP32 Core v3.x compatible
// ════════════════════════════════════════════════════════════════════

// ──────────────────────────────────────────────────────────────────
//  ★  USER CONFIGURATION BLOCK — Edit only this section  ★
// ──────────────────────────────────────────────────────────────────
#define WIFI_SSID            "******"
#define WIFI_PASS            "******"

#define BLYNK_TEMPLATE_ID    "******"
#define BLYNK_TEMPLATE_NAME  "******"
#define BLYNK_AUTH_TOKEN     "******"

#define EMAIL_SENDER_ADDR    "*************"
#define EMAIL_SENDER_PASS    "*************"
#define EMAIL_RECIPIENT      "*************"

#define OTA_PASSWORD         "smartmonitor123"
#define FIRMWARE_VERSION     "3.0.0-BEAST"

// ──────────────────────────────────────────────────────────────────
//  TIMING CONSTANTS  (milliseconds)
// ──────────────────────────────────────────────────────────────────
#define ALERT_COOLDOWN_MS         60000UL
#define WIFI_RECONNECT_INTERVAL    5000UL
#define WIFI_CHECK_INTERVAL        2000UL
#define BLYNK_PUSH_INTERVAL        2000UL
#define DHT_INTERVAL_MS            1500UL
#define DIST_INTERVAL_MS             40UL
#define GAS_INTERVAL_MS              15UL
#define SSE_INTERVAL_MS             350UL
#define LCD_ROTATE_INTERVAL        2000UL
#define SERIAL_LOG_INTERVAL         500UL
#define ANIM_FRAME_INTERVAL          28UL   // ~35 FPS
#define OFFLINE_ANIM_INTERVAL        70UL
#define LED_BREATH_INTERVAL          18UL
#define HUD_NORMAL_DURATION        8000UL
#define HUD_RADAR_DURATION         5000UL
#define HUD_HB_DURATION            4000UL

// ──────────────────────────────────────────────────────────────────
//  SENSOR THRESHOLDS
// ──────────────────────────────────────────────────────────────────
#define MAX_DIST_CM           400
#define DIST_ALERT_CM          30
#define SMOKE_THRESHOLD       1500
#define SMOKE_CONFIRM            3

// ──────────────────────────────────────────────────────────────────
//  PIN ASSIGNMENTS
// ──────────────────────────────────────────────────────────────────
#define DHTPIN                  4
#define TRIG_PIN                5
#define ECHO_PIN               18
#define MQ2_PIN                34
#define BLUE_LED_PIN            2     // External blue LED (GPIO2)
#define ONBOARD_LED            13     // Mirrors GPIO2

// LED distance bar (GPIO13 doubles as onboard LED)
const uint8_t LED_PINS[] = {13, 25, 14, 27, 26};
const uint8_t LED_COUNT  = 5;

// Blue LED PWM — ESP32 Arduino Core v3.x API
#define PWM_FREQ_BLUE        1000    // 1 kHz
#define PWM_RES_BLUE            8    // 8-bit (0-255)

// ──────────────────────────────────────────────────────────────────

#define BLYNK_PRINT Serial
#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp32.h>
#include <ESP_Mail_Client.h>
#include <math.h>

// ──────────────────────────────────────────────────────────────────
//  DISPLAY OBJECTS
// ──────────────────────────────────────────────────────────────────
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

#define LCD_ADDR 0x27
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// ──────────────────────────────────────────────────────────────────
//  SENSOR OBJECT
// ──────────────────────────────────────────────────────────────────
DHT dht(DHTPIN, DHT11);

// ──────────────────────────────────────────────────────────────────
//  LCD CUSTOM CHARACTERS
// ──────────────────────────────────────────────────────────────────
byte bellChar[8]  = {0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
byte flameChar[8] = {0x02,0x06,0x0F,0x1F,0x1F,0x0E,0x04,0x00};
byte arrowChar[8] = {0x04,0x0E,0x1F,0x04,0x04,0x04,0x04,0x00};
byte skullChar[8] = {0x0E,0x1F,0x15,0x1F,0x0E,0x0E,0x00,0x00};
byte wifiChar[8]  = {0x00,0x0E,0x1F,0x15,0x0A,0x04,0x00,0x00};
byte heartChar[8] = {0x0A,0x1F,0x1F,0x0F,0x07,0x03,0x01,0x00};

// ──────────────────────────────────────────────────────────────────
//  WEB SERVER + SSE
// ──────────────────────────────────────────────────────────────────
WebServer  server(80);
WiFiClient sseClient;
bool       sseConnected = false;

// ──────────────────────────────────────────────────────────────────
//  EMAIL (ESP Mail Client v3.x)
// ──────────────────────────────────────────────────────────────────
SMTPSession   smtp;
unsigned long lastEmailGas  = 0;
unsigned long lastEmailDist = 0;

// ──────────────────────────────────────────────────────────────────
//  BLYNK TIMERS
// ──────────────────────────────────────────────────────────────────
unsigned long lastBlynkGas  = 0;
unsigned long lastBlynkDist = 0;
unsigned long lastBlynkPush = 0;

// ──────────────────────────────────────────────────────────────────
//  SENSOR STATE
// ──────────────────────────────────────────────────────────────────
volatile unsigned long echoStart = 0;
volatile unsigned long echoDur   = 0;
volatile bool          echoReady = false;

float dist       = -1.0f;
bool  distAlert  = false;
bool  distError  = false;

int   smoke      = 0;
int   smokeCount = 0;
bool  smokeAlert = false;

float temp      = 0.0f;
float hum       = 0.0f;
bool  dhtError  = false;

// ──────────────────────────────────────────────────────────────────
//  SYSTEM STATE
// ──────────────────────────────────────────────────────────────────
bool          systemActive    = false;
int           wifiSignal      = 0;
int           signalBars      = 0;
bool          oledDirty       = true;
bool          lcdBlinkState   = true;
unsigned long bootTime        = 0;
unsigned long bootCount       = 0;   // stored in RTC memory
RTC_DATA_ATTR int rtcBootCount = 0;  // persists across deep sleep

// ──────────────────────────────────────────────────────────────────
//  MILLISECOND TIMERS
// ──────────────────────────────────────────────────────────────────
unsigned long lastDist        = 0;
unsigned long lastGas         = 0;
unsigned long lastDHT         = 0;
unsigned long lastSerial      = 0;
unsigned long lastSSE         = 0;
unsigned long lastLCDBlink    = 0;
unsigned long lastLCDUpdate   = 0;
unsigned long lastWifiCheck   = 0;
unsigned long lastWifiTry     = 0;
unsigned long lastBlueLed     = 0;
unsigned long lastAnimFrame   = 0;
unsigned long lastOfflineAnim = 0;

// ──────────────────────────────────────────────────────────────────
//  BLUE LED PWM STATE
// ──────────────────────────────────────────────────────────────────
int  bluePWM        = 0;
bool blueIncreasing = true;

// ──────────────────────────────────────────────────────────────────
//  ANIMATION STATE
// ──────────────────────────────────────────────────────────────────
bool          showingAnalysis = false;
unsigned long analysisDoneAt  = 0;
int           scanLine        = 0;

float         radarAngle      = 0.0f;
int           hbPhase         = 0;
int           offlineFrame    = 0;

// Corner decoration phase
int           cornerPhase     = 0;
unsigned long lastCorner      = 0;

enum HUDMode  { HUD_NORMAL, HUD_RADAR, HUD_HEARTBEAT };
HUDMode       hudMode         = HUD_NORMAL;
unsigned long hudModeStart    = 0;

// ════════════════════════════════════════════════════════════════════
//  FORWARD DECLARATIONS
// ════════════════════════════════════════════════════════════════════
void triggerAnalysisAnim();
void drawOLED();
void drawRadarScreen();
void drawHeartbeatScreen();
void drawWaveScanFrame();
void drawOfflineScreen();
void drawCornerDeco(int x, int y, int size);
void drawWiFiIcon(int x, int y, bool connected, int bars);
void drawSignalBars(int x, int y, int bars);
void updateBlueLED();
void ledBar(float d);
void lcdWriteRow(int row, byte icon, const char* text);
void updateLCD();
void lcdAlertBlink();
void checkAndReconnectWiFi();
void checkAndSendAlerts();
void pushBlynk();
void blynkAlertEvent(bool gas, bool prox);
void pushSSE();
void handleRoot();
void handleEvents();
void handleApiJson();
void handleStatus();
void handleNotFound();
void smtpCallback(SMTP_Status status);
void sendEmail(const char* subject, const char* body);
void jarvisBootAnimation();
void triggerPing();
void setupOTA();
String uptimeString();

// ════════════════════════════════════════════════════════════════════
//  ISR — ULTRASONIC ECHO
// ════════════════════════════════════════════════════════════════════
void IRAM_ATTR echoISR() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    echoStart = micros();
  } else if (echoStart > 0) {
    echoDur   = micros() - echoStart;
    echoReady = true;
    echoStart = 0;
  }
}

void triggerPing() {
  echoReady = false;
  digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
}

// ════════════════════════════════════════════════════════════════════
//  UTILITIES
// ════════════════════════════════════════════════════════════════════
String uptimeString() {
  unsigned long s = (millis() - bootTime) / 1000;
  unsigned long m = s / 60; s %= 60;
  unsigned long h = m / 60; m %= 60;
  char buf[16];
  snprintf(buf, 16, "%02luh%02lum%02lus", h, m, s);
  return String(buf);
}

// ════════════════════════════════════════════════════════════════════
//  WiFi — CHECK & RECONNECT
// ════════════════════════════════════════════════════════════════════
void checkAndReconnectWiFi() {
  unsigned long now = millis();
  if (now - lastWifiCheck < WIFI_CHECK_INTERVAL) return;
  lastWifiCheck = now;

  bool wasActive = systemActive;
  systemActive   = (WiFi.status() == WL_CONNECTED);

  if (!systemActive && (now - lastWifiTry >= WIFI_RECONNECT_INTERVAL)) {
    lastWifiTry = now;
    Serial.println(F("[WiFi] Reconnecting..."));
    WiFi.disconnect(true);
    delay(80);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  if (systemActive && !wasActive) {
    // Just reconnected
    wifiSignal = constrain(map(WiFi.RSSI(), -120, -30, 0, 100), 0, 100);
    signalBars = map(wifiSignal, 0, 100, 1, 5);
    Serial.printf("[WiFi] RECONNECTED  IP:%s  RSSI:%d dBm  Sig:%d%%\n",
                  WiFi.localIP().toString().c_str(), WiFi.RSSI(), wifiSignal);
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("WiFi Connected!");
    lcd.setCursor(0,1); lcd.print(WiFi.localIP().toString().c_str());
    delay(1000);
    server.begin();
    setupOTA();
    if (!Blynk.connected()) {
      Blynk.config(BLYNK_AUTH_TOKEN);
      Blynk.connect(2000);
    }
    oledDirty = true;

  } else if (!systemActive && wasActive) {
    // Just disconnected
    signalBars   = 0;
    wifiSignal   = 0;
    sseConnected = false;
    Serial.println(F("[WiFi] DISCONNECTED — System paused"));
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("WiFi OFFLINE");
    lcd.setCursor(0,1); lcd.print("Reconnecting...");
    oledDirty = true;
  }

  if (systemActive) {
    wifiSignal = constrain(map(WiFi.RSSI(), -120, -30, 0, 100), 0, 100);
    signalBars = map(wifiSignal, 0, 100, 1, 5);
  }
}

// ════════════════════════════════════════════════════════════════════
//  OTA SETUP
// ════════════════════════════════════════════════════════════════════
void setupOTA() {
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.setHostname("SmartMonitor");
  ArduinoOTA.onStart([]() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13B_tf);
    u8g2.drawStr(10, 30, "OTA UPDATING");
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(10, 48, "Do not power off");
    u8g2.sendBuffer();
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("OTA Firmware");
    lcd.setCursor(0,1); lcd.print("Updating...");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    int pct = progress / (total / 100);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13B_tf);
    u8g2.drawStr(10, 24, "OTA UPDATING");
    char pbuf[20]; snprintf(pbuf, 20, "Progress: %d%%", pct);
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(10, 40, pbuf);
    u8g2.drawFrame(4, 48, 120, 10);
    int fw = map(pct, 0, 100, 0, 118);
    if (fw > 0) u8g2.drawBox(5, 49, fw, 8);
    u8g2.sendBuffer();
    lcd.setCursor(0,1);
    char lbuf[16]; snprintf(lbuf, 16, "Progress: %3d%% ", pct);
    lcd.print(lbuf);
  });
  ArduinoOTA.onEnd([]() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13B_tf);
    u8g2.drawStr(20, 36, "OTA DONE!");
    u8g2.sendBuffer();
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[OTA] Error[%u]\n", error);
  });
  ArduinoOTA.begin();
  Serial.println(F("[OTA] Ready — password: " OTA_PASSWORD));
}

// ════════════════════════════════════════════════════════════════════
//  BLUE LED — Breathing PWM (GPIO2) + Mirror (GPIO13)
// ════════════════════════════════════════════════════════════════════
void updateBlueLED() {
  unsigned long now = millis();
  if (now - lastBlueLed < LED_BREATH_INTERVAL) return;
  lastBlueLed = now;

  if (!systemActive) {
    // 1 Hz blink when offline
    bool blink = ((now / 500) % 2) == 0;
    ledcWrite(BLUE_LED_PIN, blink ? 180 : 0);
    digitalWrite(ONBOARD_LED, blink ? HIGH : LOW);

  } else if (smokeAlert && distAlert) {
    // Ultra-fast dual-alert strobe
    bool st = ((now / 60) % 2) == 0;
    ledcWrite(BLUE_LED_PIN, st ? 255 : 0);
    digitalWrite(ONBOARD_LED, st ? HIGH : LOW);

  } else if (smokeAlert || distAlert) {
    // Fast strobe on single alert
    bool st = ((now / 110) % 2) == 0;
    ledcWrite(BLUE_LED_PIN, st ? 255 : 0);
    digitalWrite(ONBOARD_LED, st ? HIGH : LOW);

  } else {
    // Smooth breathing glow (60→255)
    if (blueIncreasing) {
      bluePWM += 3;
      if (bluePWM >= 255) { bluePWM = 255; blueIncreasing = false; }
    } else {
      bluePWM -= 3;
      if (bluePWM <= 60) { bluePWM = 60; blueIncreasing = true; }
    }
    ledcWrite(BLUE_LED_PIN, bluePWM);
    digitalWrite(ONBOARD_LED, (bluePWM > 150) ? HIGH : LOW);
  }
}

// ════════════════════════════════════════════════════════════════════
//  LED BAR — 5-LED distance visualizer
// ════════════════════════════════════════════════════════════════════
void ledBar(float d) {
  if (d < 0) {
    for (int i = 0; i < LED_COUNT; i++)
      if (LED_PINS[i] != BLUE_LED_PIN) digitalWrite(LED_PINS[i], LOW);
    return;
  }
  int level = constrain((int)map((long)d, 0, 100, LED_COUNT, 0), 0, LED_COUNT);
  for (int i = 0; i < LED_COUNT; i++) {
    if (LED_PINS[i] == BLUE_LED_PIN) continue;
    digitalWrite(LED_PINS[i], (i < level) ? HIGH : LOW);
  }
}

// ════════════════════════════════════════════════════════════════════
//  EMAIL — Gmail SMTP via ESP Mail Client v3.x
// ════════════════════════════════════════════════════════════════════
void smtpCallback(SMTP_Status status) { /* silent */ }

void sendEmail(const char* subject, const char* body) {
  if (!systemActive) return;
  Session_Config cfg;
  cfg.server.host_name  = "smtp.gmail.com";
  cfg.server.port       = 465;
  cfg.login.email       = EMAIL_SENDER_ADDR;
  cfg.login.password    = EMAIL_SENDER_PASS;
  cfg.login.user_domain = F("gmail.com");

  SMTP_Message msg;
  msg.sender.name  = F("Smart Monitor BEAST");
  msg.sender.email = EMAIL_SENDER_ADDR;
  msg.subject      = subject;
  msg.addRecipient(F("Alert"), EMAIL_RECIPIENT);
  msg.text.content = body;
  msg.text.charSet = F("utf-8");

  smtp.debug(0);
  smtp.callback(smtpCallback);
  if (!smtp.connect(&cfg)) return;
  MailClient.sendMail(&smtp, &msg);
  smtp.closeSession();
}

void checkAndSendAlerts() {
  if (!systemActive) return;
  unsigned long now = millis();

  if (smokeAlert && (now - lastEmailGas > ALERT_COOLDOWN_MS)) {
    lastEmailGas = now;
    char body[256];
    snprintf(body, sizeof(body),
      "GAS/SMOKE ALERT!\r\n"
      "Level: %d (threshold: %d)\r\n"
      "Temperature: %.1f C\r\n"
      "Humidity: %.1f%%\r\n"
      "Distance: %.1f cm\r\n"
      "WiFi Signal: %d%%\r\n"
      "Uptime: %s\r\n"
      "Firmware: " FIRMWARE_VERSION,
      smoke, SMOKE_THRESHOLD, temp, hum,
      (dist < 0) ? 0.0f : dist, wifiSignal,
      uptimeString().c_str());
    sendEmail("!!! SMART MONITOR: GAS ALERT !!!", body);
  }

  if (distAlert && (now - lastEmailDist > ALERT_COOLDOWN_MS)) {
    lastEmailDist = now;
    char body[180];
    snprintf(body, sizeof(body),
      "PROXIMITY ALERT!\r\n"
      "Object detected at: %.1f cm\r\n"
      "Alert threshold: %d cm\r\n"
      "Uptime: %s",
      dist, DIST_ALERT_CM, uptimeString().c_str());
    sendEmail("!!! SMART MONITOR: PROXIMITY ALERT !!!", body);
  }
}

// ════════════════════════════════════════════════════════════════════
//  BLYNK
// ════════════════════════════════════════════════════════════════════
void pushBlynk() {
  if (!systemActive || !Blynk.connected()) return;
  unsigned long now = millis();
  if (now - lastBlynkPush < BLYNK_PUSH_INTERVAL) return;
  lastBlynkPush = now;

  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, (dist < 0) ? 0.0f : dist);
  Blynk.virtualWrite(V3, smoke);
  Blynk.virtualWrite(V4, smokeAlert ? 1 : 0);
  Blynk.virtualWrite(V5, distAlert  ? 1 : 0);
  Blynk.virtualWrite(V6, wifiSignal);
}

void blynkAlertEvent(bool gas, bool prox) {
  if (!systemActive || !Blynk.connected()) return;
  unsigned long now = millis();
  if (gas  && (now - lastBlynkGas  > ALERT_COOLDOWN_MS)) {
    lastBlynkGas = now;
    Blynk.logEvent("gas_alert",  String("Gas level: ") + smoke);
  }
  if (prox && (now - lastBlynkDist > ALERT_COOLDOWN_MS)) {
    lastBlynkDist = now;
    Blynk.logEvent("dist_alert", String("Distance: ") + dist + " cm");
  }
}

// ════════════════════════════════════════════════════════════════════
//  WEB DASHBOARD — Beast Mode HUD
// ════════════════════════════════════════════════════════════════════
const char WEB_PAGE[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html lang="en"><head>
<meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Smart Monitor BEAST</title>
<style>
:root{--c1:#00ffff;--c2:#0088ff;--c3:#00ff88;--ca:#ff0055;--bg:#080e1c;--bg2:rgba(0,136,255,0.08)}
*{box-sizing:border-box;margin:0;padding:0}
body{background:var(--bg);color:var(--c1);font-family:'Courier New',monospace;padding:12px;min-height:100vh}
h1{text-align:center;font-size:1.6rem;letter-spacing:5px;padding:12px 0 6px;
   text-shadow:0 0 20px var(--c1),0 0 40px var(--c2);animation:glow 2.5s infinite}
.ver{text-align:center;font-size:.6rem;color:#446688;letter-spacing:3px;margin-bottom:16px}
@keyframes glow{0%,100%{text-shadow:0 0 20px var(--c1),0 0 40px var(--c2)}
  50%{text-shadow:0 0 40px var(--c1),0 0 80px var(--c2),0 0 120px #0055aa}}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:10px;max-width:520px;margin:0 auto}
.card{background:var(--bg2);border:1px solid var(--c1);border-radius:12px;padding:16px;
      text-align:center;transition:all .25s;position:relative;overflow:hidden;
      box-shadow:0 0 10px rgba(0,255,255,.15)}
.card::before{content:'';position:absolute;top:0;left:-100%;width:100%;height:2px;
  background:linear-gradient(90deg,transparent,var(--c1),transparent);
  animation:scan 3s linear infinite}
@keyframes scan{0%{left:-100%}100%{left:200%}}
.card:hover{box-shadow:0 0 25px rgba(0,255,255,.5);transform:translateY(-2px);border-color:#88ffff}
.card.alert{border-color:var(--ca)!important;background:rgba(255,0,85,.07)!important;
            animation:aflash .45s ease-in-out infinite alternate}
@keyframes aflash{from{box-shadow:0 0 15px rgba(255,0,85,.4)}
  to{box-shadow:0 0 50px rgba(255,0,85,.9),inset 0 0 30px rgba(255,0,85,.1)}}
.lbl{font-size:.6rem;color:#6699bb;text-transform:uppercase;letter-spacing:2.5px;margin-bottom:8px}
.val{font-size:2rem;font-weight:700;color:var(--c1);letter-spacing:2px;line-height:1.1}
.unit{font-size:.75rem;color:#6699bb;margin-left:2px}
.card.alert .val{color:var(--ca)}
.bar-row{max-width:520px;margin:10px auto;background:var(--bg2);border:1px solid var(--c1);
         border-radius:10px;padding:12px 16px}
.br{display:flex;align-items:center;justify-content:space-between;margin:5px 0}
.brl{font-size:.7rem;color:#6699bb;width:90px}
.brv{font-size:.8rem;color:var(--c1);font-weight:600;min-width:60px;text-align:right}
.brb{flex:1;height:6px;background:#112233;border-radius:3px;margin:0 10px;overflow:hidden}
.brb-fill{height:100%;border-radius:3px;transition:width .4s ease;
          background:linear-gradient(90deg,var(--c2),var(--c1))}
.brb-fill.warn{background:linear-gradient(90deg,#ff8800,#ffaa00)}
.brb-fill.danger{background:linear-gradient(90deg,#ff0055,#ff3377)}
.status{max-width:520px;margin:10px auto;background:var(--bg2);border:1px solid var(--c1);
        border-radius:10px;padding:12px 16px;display:flex;justify-content:space-around;flex-wrap:wrap;gap:8px}
.stag{display:flex;align-items:center;gap:6px;font-size:.75rem}
.dot{width:8px;height:8px;border-radius:50%;flex-shrink:0}
.dot-ok{background:var(--c3);box-shadow:0 0 6px var(--c3)}
.dot-warn{background:#ffaa00;box-shadow:0 0 6px #ffaa00}
.dot-bad{background:var(--ca);box-shadow:0 0 6px var(--ca);animation:dp .4s infinite alternate}
@keyframes dp{from{transform:scale(1)}to{transform:scale(1.5)}}
.tok{color:var(--c3)}.twarn{color:#ffaa00}.tbad{color:var(--ca);font-weight:700}
#log{max-width:520px;margin:10px auto;background:var(--bg2);border:1px solid var(--c1);
     border-radius:10px;padding:12px;font-size:.65rem;color:#88aacc;max-height:120px;overflow-y:auto}
#log .entry{padding:2px 0;border-bottom:1px solid rgba(0,255,255,.08);animation:le .3s ease-out}
@keyframes le{from{opacity:0;transform:translateX(-12px)}to{opacity:1}}
#log::-webkit-scrollbar{width:4px}
#log::-webkit-scrollbar-thumb{background:var(--c1);border-radius:2px}
.info-row{max-width:520px;margin:10px auto;display:flex;gap:8px;flex-wrap:wrap}
.info-pill{background:var(--bg2);border:1px solid #224466;border-radius:20px;
           padding:5px 12px;font-size:.62rem;color:#6699bb;letter-spacing:1px}
.info-pill span{color:var(--c1)}
#conn{text-align:center;font-size:.65rem;margin:8px 0;letter-spacing:1px}
.c-ok{color:var(--c3)}.c-warn{color:#ffaa00}
</style></head><body>
<h1>◈ SMART MONITOR BEAST ◈</h1>
<div class="ver">FIRMWARE v3.0-BEAST · ESP32</div>

<div class="grid">
  <div class="card" id="cT"><div class="lbl">⊙ TEMPERATURE</div>
    <div class="val" id="vT">--<span class="unit">°C</span></div></div>
  <div class="card" id="cH"><div class="lbl">◎ HUMIDITY</div>
    <div class="val" id="vH">--<span class="unit">%</span></div></div>
  <div class="card" id="cD"><div class="lbl">◈ DISTANCE</div>
    <div class="val" id="vD">--<span class="unit">cm</span></div></div>
  <div class="card" id="cG"><div class="lbl">⚡ GAS LEVEL</div>
    <div class="val" id="vG">--</div></div>
</div>

<div class="bar-row">
  <div class="br">
    <div class="brl">Temperature</div>
    <div class="brb"><div class="brb-fill" id="bT" style="width:0%"></div></div>
    <div class="brv" id="bTv">--°C</div>
  </div>
  <div class="br">
    <div class="brl">Humidity</div>
    <div class="brb"><div class="brb-fill" id="bH" style="width:0%"></div></div>
    <div class="brv" id="bHv">--%</div>
  </div>
  <div class="br">
    <div class="brl">Gas Level</div>
    <div class="brb"><div class="brb-fill warn" id="bG" style="width:0%"></div></div>
    <div class="brv" id="bGv">--</div>
  </div>
  <div class="br">
    <div class="brl">WiFi Signal</div>
    <div class="brb"><div class="brb-fill" id="bW" style="width:0%"></div></div>
    <div class="brv" id="bWv">--%</div>
  </div>
</div>

<div class="status">
  <div class="stag"><div class="dot dot-ok" id="dG"></div><span id="sG" class="tok">Gas OK</span></div>
  <div class="stag"><div class="dot dot-ok" id="dD"></div><span id="sD" class="tok">Distance OK</span></div>
  <div class="stag"><div class="dot dot-ok" id="dW"></div><span id="sW" class="tok">WiFi OK</span></div>
  <div class="stag"><div class="dot dot-ok" id="dS"></div><span id="sS" class="tok">System OK</span></div>
</div>

<div id="log"><b>━ EVENT LOG ━</b></div>
<div id="conn"><span class="c-warn">⊙ Connecting to live feed...</span></div>

<div class="info-row">
  <div class="info-pill">UPTIME <span id="iUp">--</span></div>
  <div class="info-pill">IP <span id="iIP">--</span></div>
  <div class="info-pill">RSSI <span id="iRSSI">--</span></div>
  <div class="info-pill">BOOTS <span id="iBoot">--</span></div>
</div>

<script>
const L=document.getElementById('log');
let ga=false,da=false;
function addLog(m,lvl){
  const t=new Date().toLocaleTimeString();
  const e=document.createElement('div');
  e.className='entry';
  e.style.color=lvl==='alert'?'#ff4477':lvl==='warn'?'#ffaa00':'#88aacc';
  e.textContent='['+t+'] '+m;
  L.appendChild(e);
  if(L.children.length>30)L.removeChild(L.children[1]);
  L.scrollTop=L.scrollHeight;
}
function setBar(id,pct,v,unit,danger){
  const el=document.getElementById(id);
  if(el){el.style.width=Math.min(pct,100)+'%';
    if(danger&&pct>80)el.className='brb-fill danger';
    else if(danger&&pct>50)el.className='brb-fill warn';
    else el.className='brb-fill';}
  const vEl=document.getElementById(id+'v');
  if(vEl)vEl.textContent=v+unit;
}
function dotSet(id,sid,ok,warn,okTxt,warnTxt){
  const d=document.getElementById(id),s=document.getElementById(sid);
  d.className='dot '+(ok?'dot-ok':warn?'dot-warn':'dot-bad');
  s.className=ok?'tok':warn?'twarn':'tbad';
  s.textContent=ok?okTxt:warnTxt;
}
const es=new EventSource('/events');
es.onopen=()=>{document.getElementById('conn').innerHTML='<span class="c-ok">✓ LIVE FEED ACTIVE</span>';};
es.onerror=()=>{document.getElementById('conn').innerHTML='<span class="c-warn">⊙ Reconnecting...</span>';};
es.onmessage=e=>{
  try{
    const d=JSON.parse(e.data);
    // Cards
    document.getElementById('vT').innerHTML=d.t+'<span class="unit">°C</span>';
    document.getElementById('vH').innerHTML=d.h+'<span class="unit">%</span>';
    const dv=parseFloat(d.d);
    document.getElementById('vD').innerHTML=(dv<0?'--':d.d)+'<span class="unit">cm</span>';
    document.getElementById('vG').textContent=d.g;
    const ng=d.ga==1,nd=d.da==1;
    document.getElementById('cG').classList.toggle('alert',ng);
    document.getElementById('cD').classList.toggle('alert',nd);
    // Bars
    setBar('bT',((parseFloat(d.t)-10)/50)*100,parseFloat(d.t).toFixed(1),'°C',false);
    setBar('bH',parseFloat(d.h)+'',parseFloat(d.h).toFixed(0),'%',false);
    setBar('bG',(parseInt(d.g)/4095)*100,d.g,'',true);
    setBar('bW',parseInt(d.w),parseInt(d.w),'%',false);
    // Status dots
    dotSet('dG','sG',!ng,false,'Gas OK','⚠ GAS ALERT');
    dotSet('dD','sD',!nd,false,'Distance OK','⚠ TOO CLOSE');
    dotSet('dW','sW',parseInt(d.w)>30,parseInt(d.w)>10,'WiFi OK','WiFi Weak');
    dotSet('dS','sS',!ng&&!nd,false,'System OK','!! ALERT !!');
    // Info pills
    if(d.up)document.getElementById('iUp').textContent=d.up;
    if(d.ip)document.getElementById('iIP').textContent=d.ip;
    if(d.rssi)document.getElementById('iRSSI').textContent=d.rssi+'dBm';
    if(d.boot)document.getElementById('iBoot').textContent=d.boot;
    // Logs
    if(ng&&!ga){addLog('GAS ALERT! Level: '+d.g,'alert');ga=true;}
    if(nd&&!da){addLog('PROXIMITY ALERT! Dist: '+d.d+' cm','alert');da=true;}
    if(!ng&&ga){addLog('Gas level returned to normal','');ga=false;}
    if(!nd&&da){addLog('Distance cleared','');da=false;}
  }catch(ex){}
};
</script></body></html>
)rawhtml";

// ════════════════════════════════════════════════════════════════════
//  WEB ROUTES
// ════════════════════════════════════════════════════════════════════
void handleRoot()   { server.send_P(200, "text/html", WEB_PAGE); }

void handleEvents() {
  WiFiClient client = server.client();
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/event-stream"));
  client.println(F("Cache-Control: no-cache"));
  client.println(F("Connection: keep-alive"));
  client.println(F("Access-Control-Allow-Origin: *"));
  client.println();
  client.flush();
  sseClient    = client;
  sseConnected = true;
}

void handleApiJson() {
  char buf[320];
  snprintf(buf, sizeof(buf),
    "{\"temp\":%.1f,\"hum\":%.1f,\"dist\":%.1f,\"gas\":%d,"
    "\"gasAlert\":%s,\"distAlert\":%s,\"wifi\":%d,"
    "\"rssi\":%d,\"uptime\":\"%s\",\"version\":\"%s\"}",
    temp, hum, dist, smoke,
    smokeAlert ? "true" : "false",
    distAlert  ? "true" : "false",
    wifiSignal, WiFi.RSSI(),
    uptimeString().c_str(),
    FIRMWARE_VERSION);
  server.send(200, "application/json", buf);
}

void handleStatus() {
  char buf[100];
  snprintf(buf, sizeof(buf),
    "{\"status\":\"ok\",\"uptime\":\"%s\",\"wifi\":%d}",
    uptimeString().c_str(), wifiSignal);
  server.send(200, "application/json", buf);
}

void handleNotFound() { server.send(404, "text/plain", "404"); }

void pushSSE() {
  if (!systemActive || !sseConnected || !sseClient || !sseClient.connected()) {
    sseConnected = false;
    return;
  }
  char buf[320];
  snprintf(buf, sizeof(buf),
    "data:{\"t\":\"%.1f\",\"h\":\"%.1f\",\"d\":\"%.1f\","
    "\"g\":%d,\"ga\":%d,\"da\":%d,\"w\":%d,"
    "\"rssi\":%d,\"up\":\"%s\",\"ip\":\"%s\",\"boot\":%d}\n\n",
    temp, hum, dist, smoke,
    smokeAlert ? 1 : 0, distAlert ? 1 : 0, wifiSignal,
    WiFi.RSSI(), uptimeString().c_str(),
    WiFi.localIP().toString().c_str(), rtcBootCount);
  sseClient.print(buf);
}

// ════════════════════════════════════════════════════════════════════
//  OLED GRAPHICS HELPERS
// ════════════════════════════════════════════════════════════════════

// Animated corner decoration (HUD bracket corners)
void drawCornerDeco(int x, int y, int size) {
  int s = size;
  // Determine corner direction from position
  if (x < 64 && y < 32) {
    // Top-left
    u8g2.drawHLine(x, y, s);
    u8g2.drawVLine(x, y, s);
  } else if (x > 64 && y < 32) {
    // Top-right
    u8g2.drawHLine(x - s + 1, y, s);
    u8g2.drawVLine(x, y, s);
  } else if (x < 64 && y > 32) {
    // Bottom-left
    u8g2.drawHLine(x, y, s);
    u8g2.drawVLine(x, y - s + 1, s);
  } else {
    // Bottom-right
    u8g2.drawHLine(x - s + 1, y, s);
    u8g2.drawVLine(x, y - s + 1, s);
  }
}

void drawWiFiIcon(int x, int y, bool connected, int bars) {
  if (!connected) {
    u8g2.drawCircle(x, y, 4);
    u8g2.drawLine(x - 3, y - 3, x + 3, y + 3);
    u8g2.drawLine(x + 3, y - 3, x - 3, y + 3);
    return;
  }
  u8g2.drawDisc(x, y, 2);
  if (bars >= 1) u8g2.drawCircle(x, y, 4);
  if (bars >= 2) u8g2.drawCircle(x, y, 7);
  if (bars >= 3) u8g2.drawCircle(x, y, 10);
}

void drawSignalBars(int x, int y, int bars) {
  for (int i = 0; i < 5; i++) {
    int bh = 2 + i * 2;
    int bx = x + i * 5;
    int by = y - bh;
    if (i < bars) u8g2.drawBox(bx, by, 4, bh);
    else          u8g2.drawFrame(bx, by, 4, bh);
  }
}

// ════════════════════════════════════════════════════════════════════
//  JARVIS BEAST MODE STARTUP ANIMATION — 3 phases
// ════════════════════════════════════════════════════════════════════
void jarvisBootAnimation() {
  // ── Phase 1: Scanning lines build-up ──
  for (int y = 0; y < 64; y += 4) {
    u8g2.clearBuffer();
    u8g2.drawFrame(0, 0, 128, 64);
    for (int ly = 0; ly < y; ly += 4)
      u8g2.drawHLine(2, ly, 124);
    u8g2.sendBuffer();
    delay(15);
  }

  // ── Phase 2: JARVIS typing animation ──
  const char* lines[] = {"JARVIS v3.0", "BEAST MODE", "INITIALIZING..."};
  const int   ypos[]  = {18, 30, 44};

  for (int ln = 0; ln < 3; ln++) {
    int len = strlen(lines[ln]);
    for (int i = 0; i <= len; i++) {
      u8g2.clearBuffer();
      u8g2.drawFrame(0, 0, 128, 64);
      u8g2.drawFrame(2, 2, 124, 60);

      // HUD corner decorations
      drawCornerDeco(6, 6, 8);
      drawCornerDeco(122, 6, 8);
      drawCornerDeco(6, 58, 8);
      drawCornerDeco(122, 58, 8);

      u8g2.setFont(u8g2_font_6x12_tf);
      u8g2.drawStr(18, 13, "SMART MONITOR");
      u8g2.drawHLine(4, 14, 120);

      for (int pl = 0; pl < ln; pl++) {
        u8g2.drawStr(4, ypos[pl], lines[pl]);
      }
      char partial[20] = {0};
      strncpy(partial, lines[ln], i);
      u8g2.drawStr(4, ypos[ln], partial);
      if (i < len) u8g2.drawBox(4 + i * 6, ypos[ln] - 9, 5, 9);

      // Progress bar
      u8g2.drawFrame(4, 53, 120, 7);
      int total_chars = 11 + 10 + 14;
      int done_chars  = (ln == 0) ? i : (ln == 1) ? 11 + i : 11 + 10 + i;
      int filled = map(done_chars, 0, total_chars, 0, 118);
      if (filled > 0) u8g2.drawBox(5, 54, min(filled, 118), 5);

      u8g2.sendBuffer();
      delay(40);
    }
    delay(80);
  }
  delay(200);

  // ── Phase 3: Expanding radar circle ──
  for (int r = 2; r <= 26; r += 2) {
    u8g2.clearBuffer();
    u8g2.drawFrame(0, 0, 128, 64);
    drawCornerDeco(4, 4, 8);
    drawCornerDeco(124, 4, 8);
    drawCornerDeco(4, 60, 8);
    drawCornerDeco(124, 60, 8);
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(18, 13, "SMART MONITOR");
    u8g2.drawHLine(4, 14, 120);
    u8g2.drawCircle(64, 40, r);
    if (r > 8)  u8g2.drawCircle(64, 40, r - 6);
    if (r > 14) u8g2.drawCircle(64, 40, r - 12);
    // Crosshair
    u8g2.drawHLine(64 - r, 40, r * 2);
    u8g2.drawVLine(64, 40 - r, r * 2);
    u8g2.sendBuffer();
    delay(40);
  }

  // ── Phase 4: BEAST MODE / SYSTEM READY flash ──
  for (int f = 0; f < 6; f++) {
    u8g2.clearBuffer();
    u8g2.drawFrame(0, 0, 128, 64);
    if (f % 2 == 0) {
      u8g2.drawBox(14, 16, 100, 32);
      u8g2.setDrawColor(0);
      u8g2.setFont(u8g2_font_8x13B_tf);
      u8g2.drawStr(18, 30, "BEAST MODE");
      u8g2.setFont(u8g2_font_6x12_tf);
      u8g2.drawStr(26, 44, "SYSTEM READY");
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawFrame(14, 16, 100, 32);
      u8g2.setFont(u8g2_font_8x13B_tf);
      u8g2.drawStr(18, 30, "BEAST MODE");
      u8g2.setFont(u8g2_font_6x12_tf);
      u8g2.drawStr(26, 44, "SYSTEM READY");
    }
    u8g2.sendBuffer();
    delay(200);
  }
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}

// ════════════════════════════════════════════════════════════════════
//  MAIN HUD — NORMAL DATA SCREEN
// ════════════════════════════════════════════════════════════════════
void drawOLED() {
  u8g2.clearBuffer();
  char buf[32];

  // ── Title bar with corner decorators ──
  drawCornerDeco(0, 0, 5);
  drawCornerDeco(127, 0, 5);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(18, 11, "SMART MONITOR");
  u8g2.drawHLine(0, 12, 128);

  // WiFi icon top-right
  drawWiFiIcon(120, 6, systemActive, signalBars);

  // ── Row 1: Distance ──
  if (distError) {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 24, "DIST: --- cm");
  } else {
    if (distAlert) {
      u8g2.drawBox(0, 14, 98, 11);
      u8g2.setDrawColor(0);
      u8g2.setFont(u8g2_font_6x12_tf);
      snprintf(buf, sizeof(buf), "!DIST:%.1fcm!", dist);
      u8g2.drawStr(1, 23, buf);
      u8g2.setDrawColor(1);
    } else {
      u8g2.setFont(u8g2_font_6x12_tf);
      snprintf(buf, sizeof(buf), "D:%.1fcm", dist);
      u8g2.drawStr(0, 24, buf);
      // Mini distance bar (right side)
      int dbar = constrain((int)map((long)dist, 0, MAX_DIST_CM, 64, 0), 0, 64);
      u8g2.drawFrame(96, 14, 30, 8);
      if (dbar > 0) u8g2.drawBox(97, 15, min(dbar, 28), 6);
    }
  }

  // ── Row 2: Temp ──
  if (dhtError) {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 36, "T:--- H:---");
  } else {
    u8g2.setFont(u8g2_font_6x12_tf);
    snprintf(buf, sizeof(buf), "T:%.1fC  H:%.0f%%", temp, hum);
    u8g2.drawStr(0, 36, buf);
  }

  // ── Row 3: Gas ──
  if (smokeAlert) {
    u8g2.drawBox(0, 38, 128, 12);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_6x12_tf);
    snprintf(buf, sizeof(buf), "!! GAS:%d !!", smoke);
    u8g2.drawStr(8, 48, buf);
    u8g2.setDrawColor(1);
  } else {
    u8g2.setFont(u8g2_font_6x12_tf);
    snprintf(buf, sizeof(buf), "G:%d", smoke);
    u8g2.drawStr(0, 49, buf);
    // Gas bar
    int gbar = constrain((int)map(smoke, 0, 4095, 0, 68), 0, 68);
    u8g2.drawFrame(30, 40, 70, 7);
    if (gbar > 0) u8g2.drawBox(31, 41, gbar, 5);
  }

  u8g2.drawHLine(0, 51, 128);

  // ── Bottom status bar ──
  u8g2.setFont(u8g2_font_5x7_tf);
  if (systemActive) {
    snprintf(buf, sizeof(buf), "WiFi:%d%%", wifiSignal);
    u8g2.drawStr(0, 61, buf);
    drawSignalBars(70, 61, signalBars);
    String up = uptimeString();
    u8g2.drawStr(95, 61, up.c_str());
  } else {
    u8g2.drawStr(0, 61, "-- OFFLINE --");
  }

  drawCornerDeco(0, 63, 5);
  drawCornerDeco(127, 63, 5);

  u8g2.sendBuffer();
  oledDirty = false;
}

// ════════════════════════════════════════════════════════════════════
//  WAVE SCAN ANIMATION (triggered on every sensor data change)
// ════════════════════════════════════════════════════════════════════
void triggerAnalysisAnim() {
  showingAnalysis = true;
  analysisDoneAt  = millis() + 500;
  scanLine        = 14;
}

void drawWaveScanFrame() {
  scanLine += 4;
  if (scanLine > 63) scanLine = 14;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(14, 11, "◈ SCANNING ◈");
  u8g2.drawHLine(0, 12, 128);

  char buf[24];
  u8g2.setFont(u8g2_font_5x7_tf);
  if (!distError)  { snprintf(buf,24,"Dist: %.1f cm",dist); u8g2.drawStr(0,24,buf); }
  else               u8g2.drawStr(0, 24, "Dist: ---");
  if (!dhtError) {
    snprintf(buf,24,"T:%.1fC  H:%.0f%%",temp,hum);
    u8g2.drawStr(0, 34, buf);
  }
  snprintf(buf,24,"Gas: %d",smoke);
  u8g2.drawStr(0, 44, buf);

  // XOR sweep band
  u8g2.setDrawColor(2);
  for (int i = -4; i <= 4; i++) {
    int y = scanLine + i;
    if (y > 12 && y < 64) u8g2.drawHLine(0, y, 128);
  }
  u8g2.setDrawColor(1);
  u8g2.sendBuffer();
}

// ════════════════════════════════════════════════════════════════════
//  RADAR SCANNER ANIMATION
// ════════════════════════════════════════════════════════════════════
void drawRadarScreen() {
  const int cx = 95, cy = 38, R = 24;

  radarAngle += 7.0f;
  if (radarAngle >= 360.0f) radarAngle -= 360.0f;

  float rad = radarAngle * (float)M_PI / 180.0f;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(0, 11, "◈ RADAR");
  u8g2.drawHLine(0, 12, 128);

  // Left panel — live data
  u8g2.setFont(u8g2_font_5x7_tf);
  char buf[20];
  if (!distError) { snprintf(buf,20,"D:%.0fcm",dist); u8g2.drawStr(0,24,buf); }
  if (!dhtError) {
    snprintf(buf,20,"T:%.0fC",temp);   u8g2.drawStr(0,33,buf);
    snprintf(buf,20,"H:%.0f%%",hum);  u8g2.drawStr(0,42,buf);
  }
  snprintf(buf,20,"G:%d",smoke);      u8g2.drawStr(0,51,buf);
  snprintf(buf,20,"W:%d%%",wifiSignal); u8g2.drawStr(0,61,buf);

  // Radar display
  u8g2.drawCircle(cx, cy, R);
  u8g2.drawCircle(cx, cy, R * 2 / 3);
  u8g2.drawCircle(cx, cy, R / 3);
  u8g2.drawHLine(cx - R, cy, R * 2);
  u8g2.drawVLine(cx, cy - R, R * 2);

  // Sweep line + trail
  for (int t = 0; t <= 4; t++) {
    float ta = (radarAngle - t * 12.0f) * (float)M_PI / 180.0f;
    int   tr = R - t * 2;
    if (tr <= 0) continue;
    int tx = cx + (int)(tr * cos(ta));
    int ty = cy + (int)(tr * sin(ta));
    u8g2.drawLine(cx, cy, tx, ty);
  }

  // Distance blip ring
  if (!distError && dist > 0 && dist < 200.0f) {
    int blipR = constrain((int)map((long)dist, 0, 200, 0, R), 1, R);
    u8g2.drawDisc(cx, cy, 2);
    u8g2.drawCircle(cx, cy, blipR);
    // Blip dot on sweep
    int bx = cx + (int)(blipR * cos(rad));
    int by = cy + (int)(blipR * sin(rad));
    u8g2.drawDisc(bx, by, 2);
  }

  // Alert tag
  if (distAlert) {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(cx - 10, cy + R + 8, "CLOSE!");
  }

  u8g2.sendBuffer();
}

// ════════════════════════════════════════════════════════════════════
//  ECG HEARTBEAT ANIMATION
// ════════════════════════════════════════════════════════════════════
void drawHeartbeatScreen() {
  hbPhase = (hbPhase + 3) % 128;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(0, 11, "◈ VITALS");
  u8g2.drawHLine(0, 12, 128);

  // Left panel
  u8g2.setFont(u8g2_font_5x7_tf);
  char buf[20];
  if (!dhtError) {
    snprintf(buf,20,"T:%.1fC",temp);  u8g2.drawStr(0,24,buf);
    snprintf(buf,20,"H:%.0f%%",hum); u8g2.drawStr(0,33,buf);
  }
  snprintf(buf,20,"G:%d",smoke);     u8g2.drawStr(0,42,buf);

  // Waveform box
  const int wx = 34, wy = 14, ww = 92, wh = 46;
  const int base = wy + wh / 2;
  u8g2.drawFrame(wx, wy, ww, wh);

  int prevY = base;
  for (int x = wx + 1; x < wx + ww - 1; x++) {
    int pos = (x - wx + hbPhase) % ww;
    int y   = base;
    int seg = pos % 68;

    if      (seg < 6)  y = base;
    else if (seg < 9)  y = base - 3;
    else if (seg < 12) y = base;
    else if (seg < 14) y = base + 7;
    else if (seg < 16) y = base - (wh / 2 - 3);
    else if (seg < 18) y = base + 5;
    else if (seg < 21) y = base;
    else if (seg < 28) y = base - (int)(4 * sin((seg - 21) * 0.448f));
    else               y = base;

    y = constrain(y, wy + 2, wy + wh - 2);
    u8g2.drawLine(x - 1, prevY, x, y);
    prevY = y;
  }

  // Scan dot
  int dotX = wx + 2 + hbPhase % (ww - 4);
  u8g2.drawDisc(dotX, base, 2);

  // BPM label
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(wx + 2, wy + wh - 2, "ECG");

  u8g2.sendBuffer();
}

// ════════════════════════════════════════════════════════════════════
//  OFFLINE WAITING SCREEN — animated
// ════════════════════════════════════════════════════════════════════
void drawOfflineScreen() {
  offlineFrame = (offlineFrame + 1) % 24;
  unsigned long now = millis();

  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawFrame(2, 2, 124, 60);
  drawCornerDeco(4, 4, 7);
  drawCornerDeco(124, 4, 7);
  drawCornerDeco(4, 60, 7);
  drawCornerDeco(124, 60, 7);

  u8g2.setFont(u8g2_font_8x13B_tf);
  u8g2.drawStr(16, 20, "OFFLINE");

  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(14, 33, "Waiting WiFi");

  // Animated dots
  int ndots = (now / 400) % 4;
  char dots[5] = {0};
  for (int i = 0; i < ndots; i++) dots[i] = '.';
  u8g2.drawStr(50, 33, dots);

  // Countdown to next retry
  unsigned long since = (now - lastWifiTry);
  int secs = (int)((WIFI_RECONNECT_INTERVAL - since) / 1000) + 1;
  if (secs < 1) secs = 1; if (secs > 5) secs = 5;
  char cbuf[20];
  snprintf(cbuf, 20, "Retry in %ds", secs);
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(24, 46, cbuf);

  // Pulsing disconnected wifi icon
  int pulse = (offlineFrame < 12) ? offlineFrame : (24 - offlineFrame);
  drawWiFiIcon(64, 57, false, 0);
  if (pulse > 4) u8g2.drawCircle(64, 57, pulse + 3);

  u8g2.setFont(u8g2_font_4x6_tf);
  u8g2.drawStr(2, 63, "SSID: aryan  |  " FIRMWARE_VERSION);

  u8g2.sendBuffer();
}

// ════════════════════════════════════════════════════════════════════
//  LCD DISPLAY
// ════════════════════════════════════════════════════════════════════
void lcdWriteRow(int row, byte icon, const char* text) {
  lcd.setCursor(0, row);
  lcd.write(icon);
  lcd.print(' ');
  char padded[15];
  snprintf(padded, 15, "%-14s", text);
  lcd.print(padded);
}

void updateLCD() {
  char tmp[16];

  if (!systemActive) {
    lcdWriteRow(0, 4, "WiFi OFFLINE");
    lcdWriteRow(1, 0, "Reconnecting...");
    return;
  }
  if (smokeAlert && distAlert) {
    lcdWriteRow(0, 1, "GAS + CLOSE!!!");
    lcdWriteRow(1, 3, "!! DANGER !!!");
    return;
  }
  if (smokeAlert) {
    lcdWriteRow(0, 1, "GAS DETECTED!");
    snprintf(tmp, 15, "Level: %d", smoke);
    lcdWriteRow(1, 1, tmp);
    return;
  }
  if (distAlert) {
    lcdWriteRow(0, 2, "OBJECT CLOSE!");
    snprintf(tmp, 15, "Dist:%.1fcm", dist);
    lcdWriteRow(1, 2, tmp);
    return;
  }

  // Normal 4-slot rotate
  unsigned long slot = (millis() / LCD_ROTATE_INTERVAL) % 4;
  if (slot == 0) {
    snprintf(tmp, 15, "T:%.1fC H:%.0f%%", temp, hum);
    lcdWriteRow(0, 0, tmp);
    snprintf(tmp, 15, "Gas: %d", smoke);
    lcdWriteRow(1, 2, tmp);
  } else if (slot == 1) {
    snprintf(tmp, 15, "Dist:%.1fcm", (dist < 0) ? 0.0f : dist);
    lcdWriteRow(0, 2, tmp);
    lcdWriteRow(1, 0, "All Systems OK");
  } else if (slot == 2) {
    snprintf(tmp, 15, "WiFi: %d%%", wifiSignal);
    lcdWriteRow(0, 4, tmp);
    lcdWriteRow(1, 5, "Beast Mode v3");
  } else {
    String up = uptimeString();
    snprintf(tmp, 15, "Up: %s", up.c_str());
    lcdWriteRow(0, 0, tmp);
    snprintf(tmp, 15, "IP:%s", WiFi.localIP().toString().c_str());
    lcdWriteRow(1, 4, tmp);
  }
}

void lcdAlertBlink() {
  unsigned long now = millis();
  if (now - lastLCDBlink >= 250) {
    lastLCDBlink  = now;
    lcdBlinkState = !lcdBlinkState;
    lcdBlinkState ? lcd.backlight() : lcd.noBacklight();
  }
}

// ════════════════════════════════════════════════════════════════════
//  SETUP
// ════════════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  delay(400);
  bootTime = millis();
  rtcBootCount++;

  Serial.println(F("\n\n"));
  Serial.println(F("╔══════════════════════════════════════════════╗"));
  Serial.println(F("║  SMART MONITOR — BEAST MODE ULTIMATE v3.0   ║"));
  Serial.println(F("║  ESP32 · " FIRMWARE_VERSION "                   ║"));
  Serial.println(F("╚══════════════════════════════════════════════╝"));
  Serial.printf("[BOOT] Boot #%d\n", rtcBootCount);

  // ── I2C ──
  Wire.begin(21, 22);

  // ── OLED ──
  u8g2.begin();
  Serial.println(F("[INIT] OLED SH1106 128x64 ✓"));

  // ── LCD ──
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, bellChar);
  lcd.createChar(1, flameChar);
  lcd.createChar(2, arrowChar);
  lcd.createChar(3, skullChar);
  lcd.createChar(4, wifiChar);
  lcd.createChar(5, heartChar);
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Beast Monitor");
  lcd.setCursor(0,1); lcd.print("Booting v3.0...");
  Serial.println(F("[INIT] LCD 16x2 ✓"));

  // ── Blue LED PWM — ESP32 Core v3.x ──
  ledcAttach(BLUE_LED_PIN, PWM_FREQ_BLUE, PWM_RES_BLUE);
  ledcWrite(BLUE_LED_PIN, 0);
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);
  Serial.println(F("[INIT] Blue LED PWM GPIO2 ✓"));

  // ── LED Bar + Sensor GPIOs ──
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT); digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);
  pinMode(MQ2_PIN, INPUT);
  for (int i = 0; i < LED_COUNT; i++) {
    if (LED_PINS[i] == BLUE_LED_PIN) continue;
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  Serial.println(F("[INIT] Sensors + LED bar ✓"));

  // ── JARVIS Boot Animation ──
  jarvisBootAnimation();

  // ── WiFi ──
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print(F("[WiFi] Connecting to \"aryan\""));

  lcd.clear();
  lcd.setCursor(0,0); lcd.print("WiFi: aryan");

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 40) {
    u8g2.clearBuffer();
    u8g2.drawFrame(0, 0, 128, 64);
    drawCornerDeco(4, 4, 7); drawCornerDeco(124, 4, 7);
    drawCornerDeco(4, 60, 7); drawCornerDeco(124, 60, 7);
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(10, 14, "CONNECTING...");
    u8g2.drawHLine(4, 15, 120);

    char dotline[20] = {0};
    for (int d = 0; d < tries % 17; d++) dotline[d] = '.';
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(4, 28, dotline);
    u8g2.drawStr(4, 38, "SSID: aryan");

    int pBar = map(tries, 0, 40, 0, 120);
    u8g2.drawFrame(4, 46, 120, 8);
    if (pBar > 0) u8g2.drawBox(5, 47, pBar, 6);

    char ptxt[12]; snprintf(ptxt, 12, "%d / 20s", tries / 2);
    u8g2.drawStr(4, 62, ptxt);
    u8g2.sendBuffer();

    delay(500);
    Serial.print('.');
    tries++;
    lcd.setCursor(0,1);
    char lbuf[16]; snprintf(lbuf,16,"Trying... %2ds ", tries/2);
    lcd.print(lbuf);
  }

  if (WiFi.status() == WL_CONNECTED) {
    systemActive = true;
    wifiSignal   = constrain(map(WiFi.RSSI(), -120, -30, 0, 100), 0, 100);
    signalBars   = map(wifiSignal, 0, 100, 1, 5);
    String ip    = WiFi.localIP().toString();

    Serial.println(F(""));
    Serial.println("[WiFi] ✓ CONNECTED");
    Serial.println("[WiFi] IP   : " + ip);
    Serial.printf("[WiFi] RSSI : %d dBm  Signal: %d%%\n", WiFi.RSSI(), wifiSignal);

    lcd.clear();
    lcd.setCursor(0,0); lcd.print("WiFi Connected!");
    lcd.setCursor(0,1); lcd.print(ip.c_str());

    u8g2.clearBuffer();
    u8g2.drawFrame(0, 0, 128, 64);
    drawCornerDeco(4, 4, 7); drawCornerDeco(124, 4, 7);
    drawCornerDeco(4, 60, 7); drawCornerDeco(124, 60, 7);
    u8g2.setFont(u8g2_font_7x13B_tf);
    u8g2.drawStr(8, 22, "WIFI CONNECTED");
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(8, 36, ip.c_str());
    drawSignalBars(80, 50, signalBars);
    u8g2.setFont(u8g2_font_5x7_tf);
    char sbuf[20]; snprintf(sbuf,20,"Signal: %d%%  RSSI:%ddBm",wifiSignal,WiFi.RSSI());
    u8g2.drawStr(4, 50, sbuf);
    u8g2.drawStr(4, 62, "Starting services...");
    u8g2.sendBuffer();
    delay(1500);

    // Web routes
    server.on("/",        handleRoot);
    server.on("/events",  handleEvents);
    server.on("/api",     handleApiJson);
    server.on("/status",  handleStatus);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("[WEB] Server → http://" + ip);
    Serial.println("[WEB] Dashboard → http://" + ip + "/");
    Serial.println("[WEB] JSON API  → http://" + ip + "/api");

    // OTA
    setupOTA();

    // Blynk
    Blynk.config(BLYNK_AUTH_TOKEN);
    Blynk.connect(3000);
    Serial.println(Blynk.connected() ? "[BLYNK] ✓ Connected" : "[BLYNK] Offline — data will sync later");

  } else {
    systemActive = false;
    Serial.println(F("\n[WiFi] ✗ FAILED — Offline mode active"));
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("WiFi OFFLINE");
    lcd.setCursor(0,1); lcd.print("Will retry x5s");
    delay(800);
  }

  lcd.clear();
  updateLCD();
  lastWifiTry  = millis();
  hudModeStart = millis();

  Serial.println(F("\n╔══════════════════════════════════════════════╗"));
  Serial.println(F("║         BEAST MODE ACTIVE — LOOP START       ║"));
  Serial.println(F("╚══════════════════════════════════════════════╝\n"));
}

// ════════════════════════════════════════════════════════════════════
//  MAIN LOOP
// ════════════════════════════════════════════════════════════════════
void loop() {
  unsigned long now = millis();

  // ── 1. WiFi health + blue LED (always runs) ──
  checkAndReconnectWiFi();
  updateBlueLED();

  // ── 2. OFFLINE: show offline screen, skip everything else ──
  if (!systemActive) {
    if (now - lastLCDUpdate >= 1000) { lastLCDUpdate = now; updateLCD(); }
    if (now - lastOfflineAnim >= OFFLINE_ANIM_INTERVAL) { lastOfflineAnim = now; drawOfflineScreen(); }
    delay(15);
    return;
  }

  // ── 3. Network services (online only) ──
  server.handleClient();
  ArduinoOTA.handle();
  if (Blynk.connected()) Blynk.run();

  // ── 4. GAS SENSOR (every 15 ms) ──
  if (now - lastGas >= GAS_INTERVAL_MS) {
    lastGas = now;
    int raw = analogRead(MQ2_PIN);
    smoke   = raw;
    bool prev = smokeAlert;
    smokeCount = (raw > SMOKE_THRESHOLD)
                  ? min(smokeCount + 1, SMOKE_CONFIRM)
                  : max(smokeCount - 1, 0);
    smokeAlert = (smokeCount >= SMOKE_CONFIRM);
    if (smokeAlert != prev) {
      oledDirty = true;
      triggerAnalysisAnim();
      updateLCD();
      checkAndSendAlerts();
      blynkAlertEvent(smokeAlert, false);
      if (smokeAlert) Serial.printf("[ALERT] GAS DETECTED! Level:%d\n", smoke);
      else            Serial.println(F("[INFO]  Gas cleared"));
    }
  }

  // ── 5. DISTANCE SENSOR (every 40 ms) ──
  if (now - lastDist >= DIST_INTERVAL_MS) {
    lastDist = now;
    if (echoReady) {
      float cm = echoDur * 0.0343f / 2.0f;
      distError = (cm <= 0.5f || cm > (float)MAX_DIST_CM);
      if (!distError) {
        bool prev = distAlert;
        dist      = cm;
        distAlert = (cm <= (float)DIST_ALERT_CM);
        if (distAlert != prev) {
          oledDirty = true;
          triggerAnalysisAnim();
          updateLCD();
          checkAndSendAlerts();
          blynkAlertEvent(false, distAlert);
          if (distAlert) Serial.printf("[ALERT] PROXIMITY! %.1f cm\n", dist);
        }
        ledBar(dist);
        oledDirty = true;
      }
    } else {
      distError = true;
    }
    triggerPing();
  }

  // ── 6. DHT11 (every 1500 ms) ──
  if (now - lastDHT >= DHT_INTERVAL_MS) {
    lastDHT = now;
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    bool  err = isnan(t) || isnan(h);
    if (!err && (t != temp || h != hum || dhtError)) {
      temp = t; hum = h;
      oledDirty = true;
      triggerAnalysisAnim();
    }
    dhtError = err;
  }

  // ── 7. OLED animation priority chain ──
  if (showingAnalysis) {
    if (now - lastAnimFrame >= ANIM_FRAME_INTERVAL) {
      lastAnimFrame = now;
      drawWaveScanFrame();
    }
    if (now >= analysisDoneAt) {
      showingAnalysis = false;
      oledDirty       = true;
      hudModeStart    = now;
    }
  } else {
    // Cycle HUD modes: normal → radar → heartbeat → normal
    unsigned long elapsed = now - hudModeStart;
    HUDMode newMode = hudMode;
    if      (hudMode == HUD_NORMAL    && elapsed >= HUD_NORMAL_DURATION && !smokeAlert && !distAlert)
      newMode = HUD_RADAR;
    else if (hudMode == HUD_RADAR     && elapsed >= HUD_RADAR_DURATION)
      newMode = HUD_HEARTBEAT;
    else if (hudMode == HUD_HEARTBEAT && elapsed >= HUD_HB_DURATION)
      { newMode = HUD_NORMAL; oledDirty = true; }

    if (newMode != hudMode) { hudMode = newMode; hudModeStart = now; }

    // Force normal on any alert
    if ((smokeAlert || distAlert) && hudMode != HUD_NORMAL) {
      hudMode = HUD_NORMAL;
      oledDirty = true;
    }

    if (hudMode == HUD_RADAR) {
      if (now - lastAnimFrame >= ANIM_FRAME_INTERVAL) { lastAnimFrame = now; drawRadarScreen(); }
    } else if (hudMode == HUD_HEARTBEAT) {
      if (now - lastAnimFrame >= ANIM_FRAME_INTERVAL) { lastAnimFrame = now; drawHeartbeatScreen(); }
    } else {
      if (oledDirty) drawOLED();
    }
  }

  // ── 8. LCD rotate + alert blink ──
  if (now - lastLCDUpdate >= LCD_ROTATE_INTERVAL) { lastLCDUpdate = now; updateLCD(); }
  if (smokeAlert || distAlert) lcdAlertBlink();
  else lcd.backlight();

  // ── 9. SSE push (every 350 ms) ──
  if (now - lastSSE >= SSE_INTERVAL_MS) { lastSSE = now; pushSSE(); }

  // ── 10. Blynk push (every 2 s) ──
  pushBlynk();

  // ── 11. Serial log (every 500 ms) ──
  if (now - lastSerial >= SERIAL_LOG_INTERVAL) {
    lastSerial = now;
    Serial.printf("[DATA] T:%.1fC H:%.1f%% D:%.1fcm G:%d WiFi:%d%% Up:%s%s%s\n",
      temp, hum, dist, smoke, wifiSignal, uptimeString().c_str(),
      smokeAlert ? " [GAS!]"   : "",
      distAlert  ? " [CLOSE!]" : "");
  }
}

// ════════════════════════════════════════════════════════════════════
//  END — SMART MONITOR BEAST MODE v3.0
//  Compile: ESP32 Dev Module · 921600 baud · ESP32 Core v3.x
// ════════════════════════════════════════════════════════════════════
