#include <SPI.h>
#include <SD.h>
#include <Adafruit_MPL115A2.h>


File myFile;
Adafruit_MPL115A2 mpl115a2;
unsigned long time1;
int pinCS = 10;
char data = 0;
int Clock = 0; //Timer

//SD Card
int chipSelect =  10, TestNumber = 0;
String DataString = "", FileName = "";

void setup() {
  Serial.begin(9600);
  pinMode(8,OUTPUT); //SET PIN TO OUTPUT
  mpl115a2.begin();
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  }
  else
  {
    Serial.println("SD card initialization failed");
    return;
  }

}

void loop() 
{
  float pressureKPA = 0, temperatureC = 0;

  if(Serial.available() > 0)  // Send data only when you receive data:
    {
      
      
      data = Serial.read();      //Read the incoming data and store it into variable data
      Serial.print(data);        //Print Value inside data in Serial monitor
      Serial.print("\n");        //New line 
      
      while (data =='1')
      {
           FileName = "datalog" + String(TestNumber) + ".CSV";
           File dataFile = SD.open(FileName, FILE_WRITE);
           digitalWrite(8, HIGH);
           pressureKPA = mpl115a2.getPressure();
           temperatureC = mpl115a2.getTemperature();
           if (Clock == 0) 
           {
                //First Line in the Excel Sheet
                dataFile.println("MPL115A2 (kPa), (C): ");
                dataFile.close();
           }
           DataString = String(Clock) + "," + String(pressureKPA, 4) + "," + String(temperatureC, 1);
           dataFile.println(DataString);//Print Sensor Data to SD Card
           dataFile.close();

          if (data == '0') 
          {

             digitalWrite(8, LOW);
             TestNumber = TestNumber + 1;//Prepare for a new TestFile
             
             Clock = 0;//Reset Clock
 
             break;
          }
     }
   }
}
