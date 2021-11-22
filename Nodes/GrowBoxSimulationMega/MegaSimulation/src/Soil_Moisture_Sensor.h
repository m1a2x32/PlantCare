#include "Arduino.h"
#include <math.h>

class ISoilMoistureSensor {
  private:
    long long prevUpdate = 0;
    float soilMoisture;
    const float minSoilMoisture = 0;
    const float maxSoilMoisture = 100;
    const float drainRate = 5; //In %/minute
    const float wateringRate = 25; //In %/minute
    const int updateTime = 1000;
    bool isWatering = false;
  public:
  ISoilMoistureSensor();
    float GetSoilMoisture();
    void SetIsWatering(bool IsWatering);
};
