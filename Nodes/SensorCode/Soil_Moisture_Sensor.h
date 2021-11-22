#include "Arduino.h"
#define soil_pin A9

class SoilMoistureSensor {
  private:
    float soilVal, soilMoisture;
  public:
    float GetSoilMoisture();
    void Setup();
};
