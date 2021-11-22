#include "Sensors.h"

SensorShield::SensorShield()
{
  randomSeed(analogRead(A0));
}

void SensorShield::SendData()
{
  String message = "$SendData:" + String(humidity.GetHumidity()) + ':' + String(ldr.GetLDR()) + ':' + String(temperature.GetTemperature()) + ':' + String(soilMoisture.GetSoilMoisture()) + '#';
  Serial2.println(message);
  Serial.println(message);
}

void SensorShield::CheckReadings()
{
  Serial2.println("$Water:" + String(soilMoisture.GetSoilMoisture()) + ':' + String(newProgram.GetPriority()) + '#');
}