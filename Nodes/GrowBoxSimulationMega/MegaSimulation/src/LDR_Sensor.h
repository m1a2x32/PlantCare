#include <Arduino.h>

class ILDRSensor {
  private:
    const int minLDR = 0;
    const int maxLDR = 1000;
  public:
    float GetLDR();
};
