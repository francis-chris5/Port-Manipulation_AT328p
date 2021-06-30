
/**
 * SIZE: 1770 Bytes
 */

int red = 7;
int blue = 6;
int green = 5;
int button = 2;
int pot = A2;

bool state;


void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT);
  pinMode(pot, INPUT);
  state = false;
  attachInterrupt(0, flashGreen, CHANGE);
}//end setup()




void loop() {
  state = !state;
  int level = map(analogRead(pot), 0, 1023, 255, 0);
  if(state){
    digitalWrite(red, HIGH);
    analogWrite(blue, level);
  }
  else{
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
  }
  
  
  delay(750);
}//end loop()



void flashGreen(){
  if(digitalRead(button) == HIGH){
    digitalWrite(green, HIGH);
  }
  else{
    digitalWrite(green, LOW);
  }
}
