#include <dht.h>

#define dht22_pin 12

class HumiditySensor {
  private:
    dht DHT;
  public:
    int GetHumidity();
    void Setup();
};
