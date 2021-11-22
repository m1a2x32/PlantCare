#include "Soil_Moisture_Sensor.h"

void SoilMoistureSensor::Setup()
{
  soilVal = analogRead(soil_pin);
}

float SoilMoistureSensor::GetSoilMoisture()
{
  soilMoisture = map(soilVal, 0, 1023, 100, 0);
  return soilMoisture;
}
