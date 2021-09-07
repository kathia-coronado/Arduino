// testing current sensor 

//wasworking at -6 
int offset =-11 ;// set the correction offset value

void setup() {
  // Robojax.com voltage sensor
  Serial.begin(9600);
}

void loop() {
  // Robojax.com voltage sensor
  int volt = analogRead(A0);// read the input
  double voltage = map(volt,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
  
  voltage /=100;// divide by 100 to get the decimal values
  Serial.print("Voltage: ");
  Serial.print(voltage);//print the voltge
  Serial.println("V");

delay(1000);
  
  
}
