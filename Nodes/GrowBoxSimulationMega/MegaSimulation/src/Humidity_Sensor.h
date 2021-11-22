#include <Arduino.h>

class IHumiditySensor {
  private:
    const int minHumidity = 10;
    const int maxHumidity = 90;
  public:
    int GetHumidity();
};
