int led_13 = 13;
int led_12 = 12;

void blink_once(int led_nr){
  digitalWrite(led_nr, HIGH);
  delay(1000);
  digitalWrite(led_nr, LOW);
  delay(1000);
}

void check_blink(char val){
  if ( val == 'l') {
    digitalWrite(led_12, HIGH);
    digitalWrite(led_13, LOW);
  } else if( val == 'r'){
    digitalWrite(led_12, LOW);
    digitalWrite(led_13, HIGH);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led_12, OUTPUT);
  pinMode(led_13, OUTPUT);
  Serial.begin(115200);
  digitalWrite(led_12, LOW);
  digitalWrite(led_13, LOW);
}

void loop() {
  // put your main code here, to run repeatedly: 
  while(Serial.available()==0);
  
  char val = Serial.read();
  
  check_blink(val);
  
  //Serial.println(val);
  //check_blink(val);
  //blink_once(led_13);
  //blink_once(led_12);
   
}
