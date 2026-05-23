// ======================================================
// SMART MONITOR SYSTEM - ADVANCED EDITION 🚀
// ESP32 + OLED + DHT22 + Ultrasonic + Smoke Sensor
// ======================================================

#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

// ================= OLED =================
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ================= DHT22 =================
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ================= ULTRASONIC =================
#define TRIG_PIN 5
#define ECHO_PIN 18

// ================= MQ SMOKE SENSOR =================
#define SMOKE_PIN 34

// ================= OUTPUT DEVICES =================
#define LED_PIN 2
#define BUZZER_PIN 15

// ================= VARIABLES =================
float temperature = 0;
float humidity = 0;
float distance = 0;
int smokeValue = 0;

bool danger = false;

// ======================================================
// FUNCTION : READ ULTRASONIC DISTANCE
// ======================================================
float getDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  float dist = duration * 0.034 / 2;

  return dist;
}

// ======================================================
// FUNCTION : BOOT SCREEN
// ======================================================
void bootScreen() {

  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.drawStr(15, 20, "SMART MONITOR");
  u8g2.drawStr(32, 38, "SYSTEM");

  u8g2.setFont(u8g2_font_5x8_tr);
  u8g2.drawStr(22, 55, "INITIALIZING...");

  u8g2.sendBuffer();

  delay(2500);
}

// ======================================================
// FUNCTION : ALERT MODE
// ======================================================
void alertSystem() {

  digitalWrite(LED_PIN, HIGH);

  tone(BUZZER_PIN, 1000);
  delay(200);

  digitalWrite(LED_PIN, LOW);

  noTone(BUZZER_PIN);
  delay(200);
}

// ======================================================
// FUNCTION : SAFE MODE
// ======================================================
void safeSystem() {

  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
}

// ======================================================
// FUNCTION : OLED DISPLAY
// ======================================================
void displayData() {

  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_6x12_tr);

  u8g2.drawStr(10, 10, "SMART ENV MONITOR");

  u8g2.setCursor(0, 25);
  u8g2.print("Temp : ");
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.setCursor(0, 38);
  u8g2.print("Hum  : ");
  u8g2.print(humidity);
  u8g2.print(" %");

  u8g2.setCursor(0, 51);
  u8g2.print("Dist : ");
  u8g2.print(distance);
  u8g2.print(" cm");

  u8g2.setCursor(0, 64);
  u8g2.print("Smoke: ");
  u8g2.print(smokeValue);

  if (danger) {
    u8g2.drawStr(78, 64, "ALERT!");
  }

  u8g2.sendBuffer();
}

// ======================================================
// SETUP
// ======================================================
void setup() {

  Serial.begin(115200);

  // OLED
  u8g2.begin();

  // DHT22
  dht.begin();

  // Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Smoke Sensor
  pinMode(SMOKE_PIN, INPUT);

  // LED + Buzzer
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Boot Screen
  bootScreen();
}

// ======================================================
// LOOP
// ======================================================
void loop() {

  // ================= SENSOR READINGS =================

  temperature = dht.readTemperature();

  humidity = dht.readHumidity();

  distance = getDistance();

  smokeValue = analogRead(SMOKE_PIN);

  // ================= CHECK DANGER =================

  danger = false;

  if (temperature > 40) {
    danger = true;
  }

  if (distance < 10) {
    danger = true;
  }

  if (smokeValue > 2000) {
    danger = true;
  }

  // ================= ALERT CONTROL =================

  if (danger) {
    alertSystem();
  } else {
    safeSystem();
  }

  // ================= SERIAL MONITOR =================

  Serial.println("\n========== SMART MONITOR ==========");

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity    : ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Distance    : ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Smoke Value : ");
  Serial.println(smokeValue);

  if (danger) {
    Serial.println("STATUS      : ALERT DETECTED!");
  } else {
    Serial.println("STATUS      : SAFE");
  }

  Serial.println("===================================");

  // ================= OLED =================

  displayData();

  delay(1000);
}