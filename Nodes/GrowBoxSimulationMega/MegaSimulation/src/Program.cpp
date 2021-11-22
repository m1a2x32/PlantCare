#include "Program.h"

void Program::AssignProgram(int _duration, float _temperature, float _soilMoisture, int _humidity, float _lightIntensity, int _lightCycle, int _waterPriority)
{
  duration = _duration;
  temperature = _temperature;
  soilMoisture = _soilMoisture;
  lightIntensity = _lightIntensity;
  humidity = _humidity;
  lightCycle = _lightCycle;
  waterPriority = _waterPriority;
}

int Program::GetDuration()
{
  return duration;
}

float Program::GetTemperature()
{
  return temperature;
}

int Program::GetHumidity()
{
  return humidity;
}

float Program::GetSoilMoisture()
{
  return soilMoisture;
}

int Program::GetPriority()
{
  return waterPriority;
}

float Program::GetLightCycle()
{
  return lightCycle;
}
