#include "Temperature_Sensor.h"
#include "Humidity_Sensor.h"
#include "Soil_Moisture_Sensor.h"
#include "LDR_Sensor.h"
#include "Program.h"

//#define LIGHTFORMULA ldr.GetLDR() *
#define ledY 7
#define ledB 6
#define ledG 5
#define ledR 4
#define btn1 8



class SensorShield {
  private:
    int lightLevel = 1;
    int ledState = HIGH;
    int programValue = 0;
    const long HOURINMILLIS = 3600000;
    unsigned long prevMillis;
    unsigned long prevMillisProgram;
    unsigned long currentMillisProgram;
    TemperatureSensor temperature;
    HumiditySensor humidity;
    SoilMoistureSensor soilMoisture;
    LDRSensor ldr;
    unsigned long lastDebounceTime = 0, debounceDelay = 50;
    int reading, mode;
    int btn1State = 0, lastBtn1State = 0, intervalProgram = 1500;
  public:
    bool programAssigned = false;
    Program newProgram;
    void Setup();
    int Mode();
    void ShowData(int Mode);
    void SendData();
    void SetupLCD();
    void CheckReadings();
    int ReadButton();
    void ShowProgram();
};
