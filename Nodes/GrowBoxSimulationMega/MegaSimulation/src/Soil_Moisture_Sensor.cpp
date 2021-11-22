#include "Soil_Moisture_Sensor.h"

ISoilMoistureSensor::ISoilMoistureSensor()
{
  soilMoisture = random(0, 100);
}

float ISoilMoistureSensor::GetSoilMoisture()
{
  if(millis() > prevUpdate + updateTime)
  {
    if(isWatering) {
      float test = (-drainRate + wateringRate) * (millis() - prevUpdate) / 60000;
      soilMoisture += test;
    } else {
      float test = -drainRate * (millis() - prevUpdate) / 60000;
      soilMoisture += test;
    }
    prevUpdate = millis();
  }
  soilMoisture = min(max(soilMoisture, 0), 100);
  return soilMoisture;
}

void ISoilMoistureSensor::SetIsWatering(bool IsWatering)
{
  isWatering = IsWatering;
}