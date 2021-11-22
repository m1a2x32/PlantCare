#include "Arduino.h"
#define ntc_pin A1

class TemperatureSensor {
  private:
    const int ntc_r25 = 10000, ntc_constant = 3950;
    float resistance, temperature, ntcRawValue;
  public:
    void Setup();
    float GetTemperature();
};
