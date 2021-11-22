#include "Sensors.h"

SensorShield sensorShield;

unsigned long previousMillis;
const long interval = 5000;
unsigned long previousMillis1 = 0;
int interval1 = 2000;
int btn1State = LOW, lastBtn1State = LOW, reading;

/****Debounce*****/
unsigned long lastDebounceTime = 0, debounceDelay = 50;
void setup()
{
  sensorShield.SetupLCD();
  Serial.begin(9600);
  Serial2.begin(115200);
  pinMode(dht22_pin, INPUT);
  pinMode(ldr_pin, INPUT);
  pinMode(ntc_pin, INPUT);
  pinMode(soil_pin, INPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
}

void loop()
{
  ReadSerial();

  sensorShield.Setup();
  unsigned long currentMillis = millis();
  sensorShield.ShowData(sensorShield.Mode());
  if (currentMillis - previousMillis >= interval)
  {
    sensorShield.SendData();
    previousMillis = currentMillis;
  }
  if (sensorShield.programAssigned == true)
  {
    if (sensorShield.newProgram.GetDuration() == 0)
    {
      //Serial2.println("CycleCompleted:00#");
      sensorShield.programAssigned = false;
      digitalWrite(ledG, LOW);
      digitalWrite(ledR, LOW);
      digitalWrite(ledY, LOW);
      digitalWrite(ledB, LOW);
    }
    sensorShield.newProgram.UpdateDuration();
    sensorShield.CheckReadings();
  }

}

String Split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void ReadSerial()
{
  if (Serial2.available() > 0)
  {
    String incomingMessage = Serial2.readStringUntil('#');
    // Serial.println(incomingMessage);
    if (Split(incomingMessage, ':', 0) == "$Program")
    {
      if (sensorShield.programAssigned == false)
      {
        String data[8];
        for (int i = 1; i < 7; i++)
        {
          data[i] = Split(incomingMessage, ':', i + 1);
          Serial.println(data[i]);
        }
        sensorShield.newProgram.AssignProgram(data[1].toFloat(), data[2].toFloat(), data[3].toFloat(), data[4].toInt(), data[5].toFloat(), data[6].toFloat(), data[7].toInt());
        sensorShield.programAssigned = true;
        Serial.println("Program Assigned");
        Serial2.println("$Program:00#");
      }
      else if (sensorShield.programAssigned == true)
      {
        Serial2.println("$Program:01#");
      }
    }
    else if (Split(incomingMessage, ':', 0) == "$SendData")
    {
      if (Split(incomingMessage, ':', 1) == "01")
        sensorShield.SendData();
    }
    else if (Split(incomingMessage, ':', 0) == "$IsWatering")
    {
      if (Split(incomingMessage, ':', 1) == "1")
        digitalWrite(ledB, HIGH);
      else if (Split(incomingMessage, ':', 1) == "0")
        digitalWrite(ledB, LOW);
    }
    else
    {
      Serial2.println("$Error:");
    }
  }
}
