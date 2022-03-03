#include <Wire.h>
#include <Adafruit_MPL115A2.h>
#include <SPI.h>
#include <SD.h>


char data = 0;
int Clock = 0; //Timer
Adafruit_MPL115A2 mpl115a2;

File myfile;

void setup() {
 Serial.begin(9600);
 pinMode(8,OUTPUT); //SET PIN TO OUTPUT
 mpl115a2.begin();
}

void loop(){
  float pressureKPA = 0, temperatureC = 0;   
  time1= millis()/1000;

  pressureKPA = mpl115a2.getPressure(); 

  Serial.print(time1);
  Serial.print(", ");
  Serial.print(pressureKPA, 4);
  if(Serial.available() > 0)  // Send data only when you receive data:
    {
      
      
      data = Serial.read();      //Read the incoming data and store it into variable data
      Serial.print(data);        //Print Value inside data in Serial monitor
      Serial.print("\n");        //New line 
      
      if(data == '1')            //Checks whether value of data is equal to 1 
      digitalWrite(8, HIGH);  //If value is 1 then LED turns ON
       
      else if(data == '0')       //Checks whether value of data is equal to 0
      digitalWrite(8, LOW);   //If value is 0 then LED turns OFF
    
     }                          

  

}
