#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define INTERVAL 5000
#define SSID "Meu 4G"
#define PASSWORD "Sl@yper91"
#define MQTT_SERVER "ec2-18-228-136-3.sa-east-1.compute.amazonaws.com"
#define MQTT_PORT 1883
#define TOPIC_NAME "BPM"
#define ID "esp32"

WiFiClient wifiClient;

PubSubClient client(MQTT_SERVER, 1883, wifiClient);

PulseOximeter pox;
int bpm;

long lastPublishTime = 0;

void onBeatDetected() {

  bpm = pox.getHeartRate();
  Serial.println("Batimento Detectado!");
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  connectMQTTServer();
  if (!pox.begin()) {
    Serial.println("FALHOU!!");
    for (;;); // loop infinito estranho
  } else {
    Serial.println("SUCESSO...");

    Serial.println(WiFi.localIP());
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  long now = millis();
  pox.update();
  if (now - lastPublishTime > INTERVAL) {
    lastPublishTime = now;
    onBeatDetected();
    Serial.print("Publish message: ");
    String msg = createJsonString();
    Serial.println(msg);
    client.publish(TOPIC_NAME, msg.c_str());
  }
}

void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void connectMQTTServer() {
  Serial.println("Connecting to MQTT Server...");
  if (client.connect(ID)) {
    Serial.println("connected");
  } else {
    Serial.print("error = ");
    Serial.println(client.state());
  }
}

String createJsonString() {
  String data = "{";
  data += "\"d\": {";
  data += "\"Batimentos\":";
  data += String(bpm);
  data += ",";
  data += "}";
  return data;
}
