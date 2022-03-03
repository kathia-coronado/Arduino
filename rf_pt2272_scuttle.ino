

void setup() {
 pinMode(8,OUTPUT); //SET PIN TO OUTPUT
  Serial.begin(9600);
}



void loop(){

  if  (digitalRead(9) )
  {
    digitalWrite(8, HIGH); 
    }

  


}
