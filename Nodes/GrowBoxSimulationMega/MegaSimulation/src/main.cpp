#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Sensors.h"

SensorShield sensorShield;

String Split(String data, char separator, int index);
void ReadSerial();

const long interval = 5000;
unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(9600);
  Serial2.begin(115200);
  // put your setup code here, to run once:
}

void loop()
{
  ReadSerial();
  //sensorShield.CheckReadings();
  if (millis() >= previousMillis + interval)
  {
    sensorShield.SendData();
    previousMillis = millis();
  }
}

void ReadSerial()
{
  if (Serial2.available() > 0)
  {
    String incomingMessage = Serial2.readStringUntil('#');
    Serial.println(incomingMessage);
    if (Split(incomingMessage, ':', 0) == "$Program")
    {
      if (sensorShield.programAssigned == false)
      {
        String data[8];
        for (int i = 0; i < 7; i++)
        {
          data[i] = Split(incomingMessage, ':', i + 1);
          Serial.println(data[i]);
        }
        sensorShield.newProgram.AssignProgram(data[1].toFloat(), data[2].toFloat(), data[3].toFloat(), data[4].toInt(), data[5].toFloat(), data[6].toFloat(), data[7].toInt());
        sensorShield.programAssigned = true;
        Serial.println("Program Assigned");
        Serial2.println("$Program:00#");
        Serial2.flush();
      }
      else if (sensorShield.programAssigned == true)
      {
        Serial2.println("$Program:01#");
      }
    } 
    else if (Split(incomingMessage, ':', 0) == "$SendData")
    {
      if (Split(incomingMessage, ':', 1) == "01") {}
        //sensorShield.SendData();
    }
    else if (Split(incomingMessage, ':', 0) == "$IsWatering")
    {
      sensorShield.soilMoisture.SetIsWatering(Split(incomingMessage, ':', 1).toInt());
    } else {
      Serial2.println("$Error:");
    }
  }
}

String Split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}