int PulseSensorPurplePin = 0;
int Signal;
int Threshold = 2400;

void setup() {

  Serial.begin(115200);

}

void loop() {
  
  Signal = analogRead(PulseSensorPurplePin);                     
  Serial.println(Signal);                   
delay(1500); 

}
