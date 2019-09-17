//inserir lib do ble
//verficar se nao precisa de lib para o sensor

#define threshold 2400
int a1 = 0;
int bpm = 0;

void setup() {
  Serial.begin(115200);
  //iniciar ble
}

void loop() {
  bpm = analogRead(a1);                     
  Serial.println(bpm);                   
  delay(1500);
  //enviar bpm por bluetooth
}
