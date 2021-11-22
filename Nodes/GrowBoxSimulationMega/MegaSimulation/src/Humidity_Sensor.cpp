#include "Humidity_Sensor.h"


int IHumiditySensor::GetHumidity()
{
  return random(minHumidity, maxHumidity);
}
