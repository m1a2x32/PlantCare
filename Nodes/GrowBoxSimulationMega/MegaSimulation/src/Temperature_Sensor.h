#include "Arduino.h"

class ITemperatureSensor {
  private:
    const int minTemp = 5; 
    const int maxTemp = 35;
  public:
    ITemperatureSensor();
    float GetTemperature();
};
