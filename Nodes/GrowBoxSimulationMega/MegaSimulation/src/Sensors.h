#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Temperature_Sensor.h"
#include "Humidity_Sensor.h"
#include "Soil_Moisture_Sensor.h"
#include "LDR_Sensor.h"
#include "Program.h"

class SensorShield
{
private:
    ITemperatureSensor temperature;
    IHumiditySensor humidity;
    ILDRSensor ldr;
public:
    ISoilMoistureSensor soilMoisture;
    bool programAssigned = false;
    Program newProgram;
    SensorShield();
    void SendData();
    void CheckReadings();
};