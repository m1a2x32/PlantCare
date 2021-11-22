#include "Humidity_Sensor.h"

void HumiditySensor::Setup()
{
    DHT.read11(dht22_pin);
}

int HumiditySensor::GetHumidity()
{
  return DHT.humidity;
}
