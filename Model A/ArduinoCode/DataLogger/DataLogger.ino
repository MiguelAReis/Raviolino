#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"


const int chipSelect = 10;
int timeInterval = 5;
unsigned long lastUnix;
RTC_DS1307 rtc;
char charFileName[10];
File dataLog;


void setup() {
  rtc.begin();
  if (! rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  while (!SD.begin(chipSelect)) {
    sdError();
  }
  generateFileName();                                        //Checks for Files in SD Card generates the next file name
  dataLog = SD.open(charFileName, FILE_WRITE);               //Creates a text file with the generated name
  Serial.println(dataLog);
  dataLog.close();
}

void loop() {
  logData(String(analogRead(A0)), 5);                        //Logs data from pin A0 every 5 secs
}

void logData(String data, int timeInterval) {
  DateTime now = rtc.now();                                
  if (now.unixtime() >= lastUnix + timeInterval) {           //Checks if the timeInterval has passed since the last log
    dataLog = SD.open(charFileName, FILE_WRITE);
    dataLog.println(getTimeStamp() + "," + data);
    dataLog.close();
    SPI.end();
  }
}
String getTimeStamp() {                                      //Get timeStamp in the format DD/MM/YY HH:MM:SS
  DateTime now = rtc.now();
  lastUnix = now.unixtime();
  return String(now.day(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.year(), DEC) + " " 
  + String(now.hour(), DEC) + ":" + String(now.minute(), DEC) + ":" + String(now.second(), DEC);
}
void sdError() {                                             //Blinks Pin 13 3 times
  SPI.end();
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
}
void generateFileName() {
  String fileName = String();
  unsigned int fileIndex = 1;
  while (!fileIndex == 0) {
    fileName = "F_";
    fileName += fileIndex;
    fileName += ".txt";
    charFileName[fileName.length() + 1];
    fileName.toCharArray(charFileName, sizeof(charFileName));

    if (SD.exists(charFileName)) {
      fileIndex++;
    }
    else {
      fileIndex = 0;
    }
  }
}


