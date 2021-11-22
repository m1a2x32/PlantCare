#include "Program.h"

void Program::AssignProgram(int _duration, float _temperature, float _soilMoisture, int _humidity, float _lightIntensity, float _lightCycle, int _waterPriority)
{
  duration = _duration;
  temperature = _temperature;
  soilMoisture = _soilMoisture;
  lightIntensity = _lightIntensity;
  humidity = _humidity;
  lightCycle = _lightCycle;
  waterPriority = _waterPriority;
}

void Program::UpdateDuration()
{
  if (duration == 0)
  {
    temperature = 0;
    soilMoisture = 0;
    lightIntensity = 0;
    humidity = 0;
    lightCycle = 0;
    waterPriority = 0;
  }
  if (millis() - prevMillis >= 5000)
  {
    prevMillis = millis();
    duration--;
  }
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

float Program::GetLightIntensity()
{
  return lightIntensity;
}
