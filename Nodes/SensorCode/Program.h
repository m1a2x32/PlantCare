#include "Arduino.h"

class Program {
  private:
    const long HOURINMILLIS = 3600000;
    const long DAYINMILLIS = HOURINMILLIS * 24;
    unsigned long prevMillis;
    int duration;
    float temperature;
    float soilMoisture;
    float lightIntensity;
    int humidity;
    float lightCycle;
    int waterPriority;
  public:
    void AssignProgram(int _duration, float _temperature, float _soilMoisture, int _humidity, float _lightIntensity, float _lightCycle, int _waterPriority);
    int GetDuration();
    float GetTemperature();
    float GetSoilMoisture();
    float GetLightIntensity();
    int GetHumidity();
    int GetPriority();
    float GetLightCycle();
    void UpdateDuration();
    
};
