#ifndef WATERTANK_H
#define WATERTANK_H

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>
//#include "WaterTank.cpp"

#define MAX_WATERTANK_VALUE 7
#define STANDARD_INTAKE 5
#define SIMULATION_I2C_ADDR 42

class WaterTank
{
private:
    int maxWaterLevel = STANDARD_INTAKE;
    double waterLevel;
    float res;
public:
    WaterTank();
    WaterTank(int maxWaterTankLevel);

    double currentWaterLevel();
    void updateWaterLevel(float waterLevel);
    void updateRes(float res);
    int isWaterAvailable();
    void getWaterLevel();
    void setRes(uint8_t res);
    void setRes();
};

#endif