#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <PubSubClient.h>

#define REPORTING_PERIOD_MS 1000
#define URI "http://jsonplaceholder.typicode.com/posts"

IPAddress ip;
WiFiClient espClient;
PubSubClient client(espClient);

uint32_t tsLastReport = 0;

const char* ssid = "wifi ssid";
const char* password = "password";
const char* mqttServer = "iot.eclipse.org";
const int mqttPort = 1883;
const char* mqttUser = "abcdefg";
const char* mqttPassword = "123456";

char mensagem[30];
int bpm = 0;

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
    Serial.println("Erro na conexão WiFi...");
  }
  Serial.println("Batimento Detectado!");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.println("CONECTANDO AO WIFI...");
  }
  
  ip = WiFi.localIP();
  Serial.println(ip);

  Serial.println("CONECTADO A REDE WIFI!!");
  Serial.print("Inicializando sensor de pulso...");
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FALHOU!!");
    for (;;); // loop infinito estranho
  } else {
    Serial.println("SUCESSO...");
  }

  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);


}

void loop()
{
  reconectabroker();
  sprintf(mensagem, "MQTT ESP32 - Mensagem no. %d", bpm);
  Serial.print("Mensagem enviada: ");
  Serial.println(mensagem);

  //Envia a mensagem ao broker
  client.publish("ArduinoeCia", mensagem);
  Serial.println("Mensagem enviada com sucesso...");
  
  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Serial.print("Batimento Cardiaco: ");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm\n");
    tsLastReport = millis();

      //Incrementa o bpm
  bpm++;
  
  //Aguarda 3 segundos para enviar uma nova mensagem
  delay(3000);
  }
}

void reconectabroker()
{
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
