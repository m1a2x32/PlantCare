#include "LDR_Sensor.h"

void LDRSensor::Setup()
{
  ldrRawValue = analogRead(ldr_pin);
}

float LDRSensor::GetLDR()
{
  resistance = (float)(1023 - ldrRawValue) * 10 / ldrRawValue;
  lux = 325 * pow(resistance, -1.4);
  return lux;
}
