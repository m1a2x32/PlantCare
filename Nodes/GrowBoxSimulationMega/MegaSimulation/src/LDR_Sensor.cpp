#include "LDR_Sensor.h"

float ILDRSensor::GetLDR()
{
  return random(minLDR, maxLDR);
}
