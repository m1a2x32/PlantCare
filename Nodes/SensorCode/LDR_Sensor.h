#include "Arduino.h"
#define ldr_pin A2

class LDRSensor {
  private:
    float resistance, lux, ldrRawValue;
  public:
    float GetLDR();
    void Setup();
};
