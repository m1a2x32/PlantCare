#ifndef WATERSYSTEMGROWBOX_H
#define WATERSYSTEMGROWBOX_H
#include <WiFi.h>
#include "GrowBox.h"
struct growBoxWaterPrio
{
    int priority;
    int waterLevel;
    IPAddress ip;
    growBoxWaterPrio *next = NULL;
};

#define MAX_NUMBER_OF_GROWBOXES 100

class WaterSystemGrowBox
{
private:
    int numberOfBoxes = 0;
    int requiredWaterLevel = 100;
    int valveState = 0;
    growBoxWaterPrio *growBoxList;
    growBoxWaterPrio *thisGrowBox;
    std::shared_ptr<Growbox> growBox;
public:
    WaterSystemGrowBox(std::shared_ptr<Growbox> growBoxPtr);
    int removeGrowBox(IPAddress IP);
    int checkIfHighestPriority();
    void updateThisWaterPriority(int priority);
    int updateWaterLevel(IPAddress IP, int waterLevel, int priority);
    void updateThisWaterLevel(int moistureMeasurement);
    void communicateWaterPriority(Growbox *growBox);
    void saveWaterPrio(int moistureMeasurement, int newPrio);
    void updateValveState(int isHighestPrio);
    void sendValveState(int state);
};

#endif
