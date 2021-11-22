#include "WaterTank.h"

WaterTank::WaterTank()
{
    Wire.begin();
}

WaterTank::WaterTank(int maxWaterTankLevel)
{
    Wire.begin();
    maxWaterLevel = maxWaterTankLevel;
}

void WaterTank::updateWaterLevel(float waterLevel)
{
    this->waterLevel = waterLevel;
}

double WaterTank::currentWaterLevel()
{
    return waterLevel;
}

void WaterTank::updateRes(float res)
{
    this->res = res;
}


/// return values
/// -1  waterLevel not available or value nonsensical
///  0  water available
///  1  tank empty
int WaterTank::isWaterAvailable()
{
    if(waterLevel < 0) return -1;
    if (waterLevel > 0.01 && res > 0) return 0;
    if(waterLevel > 0.10) return 0;
    return 1;
}

void WaterTank::getWaterLevel()
{   
    Wire.requestFrom(SIMULATION_I2C_ADDR, 6);

    if(Wire.available())
    {
        String data;
        for(int i = 0; i < 5; i++)
        {
            data += (char)Wire.read();
        }
        waterLevel = data.toDouble() / maxWaterLevel * 100;
        res = Wire.read();
    }
}

void WaterTank::setRes(uint8_t res)
{
    Wire.beginTransmission(SIMULATION_I2C_ADDR);
    Wire.write(res);
    Wire.endTransmission(SIMULATION_I2C_ADDR);
}

///Set intake with the standard intake
void WaterTank::setRes()
{
    setRes(STANDARD_INTAKE);
}