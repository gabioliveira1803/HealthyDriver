#include <WiFi.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <PubSubClient.h>

#define WIFI_SSID "wifi ssid"
#define WIFI_PASSWORD "pass"

#define MQTT_SERVER "iot.eclipse.org"
#define MQTT_USER "abcdefg"
#define MQTT_PASSWORD "password"
#define MQTT_PORT 1883
#define TOPIC "BPM" //UMA SUGESTAO É MUDAR O TOPICO PARA O MAC DO DEVICE CASO POSSIVEL

#define REPORTING_PERIOD_MS 1000

IPAddress ip;
PulseOximeter pox;
WiFiClient espClient;
PubSubClient client(espClient);

int bpm = 0;
uint32_t tsLastReport = 0;


void onBeatDetected() {
  bpm = pox.getHeartRate();
  if (WiFi.status() == WL_CONNECTED) {
    connectBroker();
    client.publish(TOPIC, bpm.c_str()); // talvez tenha que converter p string antes
    Serial.println("Mensagem enviada com sucesso...");
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

  Serial.println("CONECTADO A REDE WIFI!!");
  Serial.print("Inicializando sensor de pulso...");

  if (!pox.begin()) {
    Serial.println("FALHOU!!");
    for (;;); // loop infinito estranho
  } else {
    Serial.println("SUCESSO...");
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
  // TALVEZ TENHAM QUE TIRAR ESSA PARTE DO IF
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Batimento Cardiaco: ");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm\n");
    tsLastReport = millis();
  delay(1000);
  }
}

void connectBroker() {
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) { //acho que isso é diferente
      Serial.println("Conectado ao broker!");
    } else {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
