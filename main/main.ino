// define constantes e usa interruptor do arduino para akustes de precisao
#define USE_ARDUINO_INTERRUPTS true
#define PULSE 0
#define THRESHOLD 550

// falta inserir a lib de BLE
#include <PulseSensorPlayground.h>
// cria objeto do sensor de pulso a partir da lib importada
PulseSensorPlayground pulseSensor;

void setup() {
  // inicia serial e configura pino que esta conectado o sensor e o limite
  // de medição, caso tudo ocorra bem ele imprime a mensagem
  Serial.begin(115200);
  pulseSensor.analogInput(PULSE);
  pulseSensor.setThreshold(THRESHOLD);
   if (pulseSensor.begin()) {
    Serial.println("Conectado com sucesso!");
  }
}

void loop() {
 int bpm = pulseSensor.getBeatsPerMinute();
 // armazena o valor do bpm e caso seja detectado um batimento ele imprime o
 // valor no serial e aguarda um pouco
 if (pulseSensor.sawStartOfBeat()) {
   Serial.println(bpm);
   // nessa parte tem que ser feito o envio pro BLE
 }
  delay(20);
}
