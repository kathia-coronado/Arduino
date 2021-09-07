#include <Wire.h>
#include <Adafruit_MPL115A2.h>

Adafruit_MPL115A2 mpl115a2;
unsigned long time1;
void setup(void) 
{
  Serial.begin(9600);
  //Serial.println("Hello!");
  
 // Serial.println("Getting barometric pressure ...");
  mpl115a2.begin();
}

void loop(void) 
{
  float pressureKPA = 0, temperatureC = 0;   
  time1= millis()/1000;
  
  pressureKPA = mpl115a2.getPressure();  
  //Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.println(" kPa");
  temperatureC = mpl115a2.getTemperature();  
 // Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C");
 // Serial.println ("_____________________________________________");
   Serial.print(time1);
   Serial.print(", ");
   Serial.print(pressureKPA, 4);
   Serial.print(", ");
   Serial.println(temperatureC, 1); 
   //Serial.println("\n");


  delay(3000);
}
