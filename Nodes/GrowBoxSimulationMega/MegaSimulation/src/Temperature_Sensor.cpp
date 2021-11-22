#include "Temperature_Sensor.h"

ITemperatureSensor::ITemperatureSensor()
{
  randomSeed(analogRead(0));
}

float ITemperatureSensor::GetTemperature()
{
  return random(minTemp, maxTemp);
}
