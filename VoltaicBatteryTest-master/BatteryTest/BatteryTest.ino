#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

float offset = -190.00;
//Sensors and Switches
int BtnPin = 2, BtnValue = 2, LCDBtnPin = 3; //Switches Pins and Values
int VoltagePin = 0, CurrentPin = 1, LightPin = 2; //Sensor Pins
float Voltage = 0, Current = 0, Power = 0, Light = 0; //Sensor Values
int Clock = 0; //Timer

// LCD
#define I2C_ADDR 0x27 // Define I2C Address where the SainSmart LCD is
#define BACKLIGHT_PIN 3
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7);
String LCDData;

//SD Card
int chipSelect =  10, TestNumber = 0;
String DataString = "", FileName = "";

void setup() {
  Serial.begin(9600);
  //Switches Mode
  pinMode(BtnPin, INPUT_PULLUP);
  pinMode(LCDBtnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BtnPin), blink, LOW);
  //LCD SETUP
  lcd.begin (20, 4);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  //SD Card
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

void loop() {
  LCDWrite(0, 0, "System Ready ", 0);
  LCDWrite(0, 3, "Measure", 0);
  //Save LCD power by Actuating Red Switch
  //lcd.setBacklight(!digitalRead(LCDBtnPin));
  
  //Start Measurements When Black Switch is actuated
  while (BtnValue == 1) {
    //Create a new Excel file Staritng with datalog0
    FileName = "datalog" + String(TestNumber) + ".CSV";
    File dataFile = SD.open(FileName, FILE_WRITE);
    //Save LCD power by Actuating Red Switch
    //lcd.setBacklight(!digitalRead(LCDBtnPin));
    //Record Measurements from Sensors
    LCDWrite(0, 0, "Recording Data ", 0);
    //Voltage = analogRead(VoltagePin) / 40.92;
    float volt = analogRead(A0);// read the input
    Voltage = (map(volt,0,1023, 0, 2500) + offset)/100;
    //Current = ((((analogRead(CurrentPin) * 5.0) / 1023.0) - 2.5) / 0.066)*100;
    //Current = ((((analogRead(A1) * 5.0) / 1023.0) -2.5) / 0.066)*100;
    //Current =((2.5 - (analogRead(A1)*(5.0/1024.0)))/0.185)*100;
    Current = (.0264 * analogRead(A1) -13.51);
    //if (Current < 0)
    //  Current = 0;
    Power = Current * Voltage;
    Light = 1023 - analogRead(LightPin);
    
    //Start Writing data to File
    if (Clock == 0) { //First Line in the Excel Sheet
      dataFile.println("Time,Voltage,Current,Power,Light");
      dataFile.close();
    }
    //DataString = String(Clock) + "," + String(Voltage, 4) +  "," + String(Light, 0);
    DataString = String(Clock) + "," + String(Voltage, 2) + "," + String(Current, 2) + "," + String(Power, 2) + "," + String(Light, 0);
    dataFile.println(DataString);//Print Sensor Data to SD Card
    dataFile.close();
    //Print Data to LCD
    //LCDData = String(Voltage, 2) + "V, " + String(Light, 0) + "L"; //Print Sensor Data to LCD
    LCDData = String(Voltage, 1) + "V, " + String(Current, 2) + "mA, " + String(Light, 0) + "L"; //Print Sensor Data to LCD
    LCDWrite(0, 1, LCDData, 0);
    //Increment  time on LCD every 1 minute
    LCDWrite(0, 2, "Time Elapsed:" + String(Clock) + " sec", 0);
    delay(15000);//Increment 1 minute
    Clock = Clock +15;
    if (Clock >= 1)// Stop Measurements after 1 minute if needed
      LCDWrite(0, 3, "Stop:" + FileName, 0);
    else
      LCDWrite(0, 3, "         ", 0);
    //Stop button actuated at least after 1 minute
    if (BtnValue == 0) {
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
