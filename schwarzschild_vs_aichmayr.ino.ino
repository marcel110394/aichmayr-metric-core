#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <QMC5883LCompass.h>
#include <SPI.h>
#include <SD.h>

// ───── Pins
#define GSR_PIN 34
#define BLUE_LED 25
#define GREEN_LED 26
#define BUZZER_PIN 27
#define SD_CS 5

// ───── Globale Variablen
Adafruit_MPU6050 mpu;
QMC5883LCompass compass;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
File dataFile;

float r = 10;
float G = 1;
float M = 1.0;
// float rs = 5.0; // nur für Aichmayr relevant

void setup() {
  Serial.begin(115200);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GSR_PIN, INPUT);

  mpu.begin();
  compass.init();
  compass.setCalibration(-1700, 1800, -1800, 1300, -1400, 2100);

  u8g2.begin();
  u8g2.enableUTF8Print();

  if (!SD.begin(SD_CS)) {
    Serial.println("SD-Karte nicht erkannt");
    while (1);
  }

  dataFile = SD.open("/log.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("timestamp,phi,GSR,mag_x,acc_x");
    dataFile.close();
  }
}

void loop() {
  float gsr = analogRead(GSR_PIN) / 4095.0;
  compass.read();
  float mag_x = compass.getX();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float acc_x = a.acceleration.x;

  float phi = calculatePhi(r);

  bool triggered = false;
  if (phi > 1.0) {
    digitalWrite(BLUE_LED, HIGH);
    tone(BUZZER_PIN, 1000, 100);
    triggered = true;
  } else {
    digitalWrite(BLUE_LED, LOW);
  }

  // SD Logging
  dataFile = SD.open("/log.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.printf("%lu,%.6f,%.3f,%.2f,%.2f\n", millis(), phi, gsr, mag_x, acc_x);
    dataFile.close();
  }

  // OLED Anzeige
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 12);
  u8g2.print("phi(t,r): ");
  u8g2.print(phi, 6);
  u8g2.setCursor(0, 26);
  u8g2.print("GSR: ");
  u8g2.print(gsr, 3);
  u8g2.setCursor(0, 40);
  u8g2.print("AccX: ");
  u8g2.print(acc_x, 2);
  u8g2.setCursor(0, 54);
  if (triggered) {
    u8g2.print("TRIGGER!");
  } else {
    u8g2.print("Normal...");
  }
  u8g2.sendBuffer();

  delay(1000);
}

// ───── φ(t, r) mit Schwarzschild-Metrik
float calculatePhi(float r) {
  float phi = 1.0 - (2 * G * M / r);
  return phi;
}

// ───── Alternative für Aichmayr-Metrik:
//float calculatePhi(float r) {
//  float phi = 1.0 - ((2 * G * M / r) * exp(-r / rs));
//   return phi;
//}