#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

int BtnPin = 2,      BtnValue = 2,    LCDBtnPin = 3; //Switches Pins and Values
int VoltagePin = 0, CurrentPin = 1, LightPin = 2; //Sensor Pins
float Voltage = 0, Current = 0, Power = 0, Light = 0; //Sensor Values
int Clock = 0; //Timer

//SD Card
int chipSelect =  10, TestNumber = 0;
String DataString = "", FileName = "";

// LCD
#define I2C_ADDR 0x27 // Define I2C Address where the SainSmart LCD is
#define BACKLIGHT_PIN 3
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7);
String LCDData;

float offset = -190.00;

void setup() 
{
  Serial.begin(9600);
  //switches mode
  pinMode(BtnPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BtnPin), blink, LOW);
  //LCD Setup
  lcd.begin(20,4);
  lcd.setBacklight(HIGH);
  lcd.home();
  //SD card
  LCDWrite(0, 0, "Initializing SD Card", 1);
  delay(2000);
   //Checking if SD Card is inserted
  while (!SD.begin(chipSelect))
    LCDWrite(0, 0, "SD Card Failed", 1);
  //SD card is Inserted
  LCDWrite(0, 0, "SD Card Initialized", 1);
  delay(2000);
  lcd.clear();
}

void loop() 
{
  LCDWrite(0, 0, "System Ready ", 0);
  LCDWrite(0, 3, "Measure", 0);
  while (BtnValue == 1)  
    {
      FileName = "datalog" + String(TestNumber) + ".CSV";
      File dataFile = SD.open(FileName, FILE_WRITE);
     
     //Record Measurements from Sensors
      LCDWrite(0, 0, "Recording Data ", 0);
      float volt = analogRead(A0);// read the input
      Voltage = (map(volt,0,1023, 0, 2500) + offset)/100;
      //Current = (.0264 * analogRead(A1) -13.51)*100;
      float avg=0;
      for(int i = 0; i < 1000; i++) 
      {
        avg = avg + (.0264 * analogRead(A0) -13.51);    //for the 5A mode,  
        delay(1);
      }
      Current=avg/1000;
     
      Power = Current * Voltage;
      if (Clock == 0) //first line in excel sheet
        {
           dataFile.println("Time,Voltage,Current,Power,Light");
           dataFile.close();          
        }
      DataString = String(Clock) + "," + String(Voltage, 2) + "," + String(Current, 2) + "," + String(Power, 2) ;
      dataFile.println(DataString);//Print Sensor Data to SD Card
      dataFile.close();
      
      //Print data to LCD
      LCDData = String(Voltage, 2) + "V, " + String(Current, 2) + "mA, " ; //Print Sensor Data to LCD
      LCDWrite(0, 1, LCDData, 0);
      //Increment  time on LCD
      LCDWrite(0, 2, "Time Elapsed:" + String(Clock) + " sec", 0);
      delay(15000);//Increment 15 sec  
      Clock = Clock +15;   
      if (Clock >= 86400)// Stop Measurements after 1 minute if needed
           LCDWrite(0, 3, "Stop:" + FileName, 0);
       else
      LCDWrite(0, 3, "         ", 0);
      //Stop button actuated at least after 1 minute
       if (BtnValue == 0) 
        {
           LCDWrite(0, 0, "Storing Data ", 1);
           TestNumber = TestNumber + 1;//Prepare for a new TestFile
           delay(3000);
           LCDWrite(0, 1, FileName, 0);
           delay(3000);
           Clock = 0;//Reset Clock
           lcd.clear();
           BtnValue = 2;
           break;
        }
    }
}


//Function : Set LCD Cursor Position and Write Message
void LCDWrite(int r, int c, String message, int clearscreen) {
  if (clearscreen == 1)
    lcd.clear(); //Clear screen if requested
  lcd.setCursor ( r, c );
  lcd.print(message);
}
//Function : Interrupt and Stop Data Measurements
void blink() {
  if (Clock >= 1)
    BtnValue = 0;
  else
    BtnValue = 1;
}
