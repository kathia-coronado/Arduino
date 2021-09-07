#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include <Adafruit_MPL115A2.h>

Adafruit_MPL115A2 mpl115a2;
#define ONE_WIRE_BUS 2
unsigned long time1;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celcius=0;
float Fahrenheit=0;

void setup(void)
{
  Serial.begin(9600);
  mpl115a2.begin();
  sensors.begin();
  Serial.println("    DS18B20     ||     MPL115A2");
}

void loop(void)
{ 
  
  float pressureKPA = 0, temperatureC = 0;   
  time1=millis();
  sensors.requestTemperatures(); 
  //delay(1000);
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  pressureKPA = mpl115a2.getPressure();  
  temperatureC = mpl115a2.getTemperature(); 
  Serial.print(time1/1000);
  Serial.print(", ");
  Serial.print(Celcius);
  Serial.print(", ");
  Serial.print(Fahrenheit);
  Serial.print(" || ");
  Serial.print(pressureKPA, 4);
  Serial.print(", ");
  Serial.println(temperatureC, 1); 

  delay(6000);
}
