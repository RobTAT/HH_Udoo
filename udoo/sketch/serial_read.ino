
int incomingByte = 0;
int led = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);
}

void blink(){
  digitalWrite(led,HIGH);
  delay(1000);
  digitalWrite(led,LOW);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(Serial.available()>0){
    incomingByte = Serial.read();
    if(incomingByte == 1){
      blink();
    }
  
  }
}
