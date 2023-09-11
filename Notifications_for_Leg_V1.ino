#include <M5StickCPlus.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ATTW7pUNje";
const char* password = "t8g?tfbqme8r";
const char* webhookURL = "https://maker.ifttt.com/trigger/Heb_Project/with/key/fxN2fWDgs8r_5ipkZl9YH4_r4NRbHVklz4h4Y0wppQW";

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;
float threshold = 1.5F; // Adjust this value as needed

void setup() {
  // Initialize M5StickC Plus and IMU
  M5.begin();
  M5.Imu.Init();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Set up the screen and display message
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(80, 15);
  M5.Lcd.println("IMU");
  M5.Lcd.setCursor(30, 30);
  M5.Lcd.println("  X       Y       Z");
}

void loop() {
  // Read accelerometer data
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  // Check if the acceleration exceeds the threshold
  if (accX > threshold || accY > threshold || accZ > threshold) {
    // Trigger the webhook by sending a POST request
    HTTPClient http;
    http.begin(webhookURL);
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"value1\":\"" + String(accX) + "\",\"value2\":\"" + String(accY) + "\",\"value3\":\"" + String(accZ) + "\"}";
    int httpResponseCode = http.POST(payload);
    http.end();

    // Print the response code to the screen
    M5.Lcd.setCursor(170, 60);
    M5.Lcd.print("Response: ");
    M5.Lcd.println(httpResponseCode);
  }

  // Display accelerometer values on the screen
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);

  delay(100);
}
