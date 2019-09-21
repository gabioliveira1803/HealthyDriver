#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>
#define PulseWire = 0; //this pin #12 on flora

int Threshold = 550;

PulseSensorPlayground pulseSensor;

void setup() {
  Serial.begin(115200);
  Serial.println("cheguei ate aqui");
  pulseSensor.analogInput(PulseWire);
  //pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }
}
void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    Serial.println("♥ A HeartBeat Happened ! ");
    Serial.print("BPM: ");
    Serial.println(myBPM);
  }
  delay(1000);
}
