class Program {
  private:
    int duration;
    float temperature;
    float soilMoisture;
    float lightIntensity;
    int humidity;
    int lightCycle;
    int waterPriority;
  public:
    void AssignProgram(int _duration, float _temperature, float _soilMoisture, int _humidity, float _lightIntensity, int _lightCycle, int _waterPriority);
    int GetDuration();
    float GetTemperature();
    float GetSoilMoisture();  
    float GetLightIntensity();
    int GetHumidity();
    int GetPriority();
    float GetLightCycle();
};
