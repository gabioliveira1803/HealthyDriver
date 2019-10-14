#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000
#define URI "http://jsonplaceholder.typicode.com/posts"

const char* ssid = "qwer";
const char* password = "1234qwer";

int bpm = 2;

PulseOximeter pox;
HTTPClient http;

void onBeatDetected() {
  // For bpm, a value of 0 means "invalid"
  bpm = pox.getHeartRate();
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(URI);
    http.addHeader("Content-Type", "text/plain");
    int httpStatusCode = http.POST("POSTING from ESP32");
    if (httpStatusCode > 0) {
      String response = http.getString();
      Serial.println(httpStatusCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpStatusCode);
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.print("Initializing pulse oximeter..");
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;); // loop infinito estranho
  } else {
    Serial.println("SUCCESS");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
}
