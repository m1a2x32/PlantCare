#include "Temperature_Sensor.h"

void TemperatureSensor::Setup()
{
    ntcRawValue = analogRead(ntc_pin);
}

float TemperatureSensor::GetTemperature()
{
  resistance = (float)ntcRawValue * ntc_r25 / (1023 - ntcRawValue);
  temperature = 1 / (log(resistance / ntc_r25) / ntc_constant + 1 / 298.15) - 273.15;
  return temperature;
}
