/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 7
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0x81, 0x37, 0x2A, 0xA, 0x0, 0x0, 0x17 }; //LONG ADAFRUIT 
DeviceAddress sensor2 = { 0x28, 0xEF, 0xD9, 0x1F, 0xA, 0x0, 0x0, 0x76 }; //SHORT CHINA 


void setup(void){
  Serial.begin(9600);
  sensors.begin();
}

void loop(void){ 
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  
  Serial.print("Sensor_ADAF (*C): ");
  Serial.print(sensors.getTempC(sensor1)); 
  Serial.print(" Sensor_ADAF (*F): ");
  Serial.print(sensors.getTempF(sensor1)); 
  Serial.print("   ||   ");
  Serial.print("Sensor_CHIN (*C): ");
  Serial.print(sensors.getTempC(sensor2)); 
  Serial.print(" Sensor_CHIN (*F): ");
  Serial.println(sensors.getTempF(sensor2)); 
  
  
  delay(2000);
}
