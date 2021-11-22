#include "WaterSystemGrowBox.h" 

#define REQ_WATER_LEVEL(p) (float)requiredWaterLevel / 100 * p

//Define how to get the soilMoisture from the arduino
//Define how to get the priority from the arduino

WaterSystemGrowBox::WaterSystemGrowBox(std::shared_ptr<Growbox> growBoxPtr)
{
    growBox = growBoxPtr;
    thisGrowBox = new growBoxWaterPrio;
    thisGrowBox->waterLevel = -1;
}

///Return value 1: this box has priority and can open water valve, 0: this box does not have priority and should keep water valve closed or shut it, -1: invalid waterLevel 
int WaterSystemGrowBox::checkIfHighestPriority()
{
    if(thisGrowBox->waterLevel = -1)
    {
        return -1;
    }
    if(growBoxList == 0)
    {
        return 1;
    }
    growBoxWaterPrio *tempGrowBox = growBoxList;
    while (tempGrowBox->next != 0)
    {
        if(tempGrowBox->waterLevel < thisGrowBox->waterLevel)
        {
            return 0;
        }
        if(tempGrowBox->waterLevel == thisGrowBox->waterLevel && tempGrowBox->priority < thisGrowBox->priority)
        {
            return 0;
        }
        if(tempGrowBox->waterLevel == thisGrowBox->waterLevel && tempGrowBox->priority == thisGrowBox->priority && tempGrowBox->ip < thisGrowBox->ip)
        {
            return 0;
        }
    }
    return 1;
}

void WaterSystemGrowBox::updateThisWaterPriority(int priority)
{
    thisGrowBox->priority = priority;
}

///Return value 1: value of a growbox is updated, 2: new growbox is created
int WaterSystemGrowBox::updateWaterLevel(IPAddress IP, int waterLevel, int priority)
{
    growBoxWaterPrio *tempGrowBox = growBoxList;
    if(growBoxList != 0)
    {
        if (IP == tempGrowBox->ip)
        {
            tempGrowBox->priority = priority;
            tempGrowBox->waterLevel = waterLevel;
            return 1;
        }
        while(tempGrowBox->next != 0)
        {
            if(IP == tempGrowBox->next->ip)
            {
                tempGrowBox->next->priority = priority;
                tempGrowBox->next->waterLevel = waterLevel;
                return 1;
            }
            tempGrowBox = tempGrowBox->next;
        }
    } else{
        growBoxList = new growBoxWaterPrio;
        growBoxList->ip = IP;
        growBoxList->priority = priority;
        growBoxList->waterLevel = waterLevel;
        return 2;
    }
    tempGrowBox->next = new growBoxWaterPrio;
    tempGrowBox->next->ip = IP;
    tempGrowBox->next->priority = priority;
    tempGrowBox->next->waterLevel = waterLevel;
    return 2;
}

///Return value 1: grow box is successfulky removed -1: id does not match any grow boxes registered
int WaterSystemGrowBox::removeGrowBox(IPAddress IP)
{
    if(growBoxList == 0)
    {
        return -1;
    }
    growBoxWaterPrio *tempGrowBox = growBoxList;
    if(tempGrowBox->next != 0)
    {
        if(tempGrowBox->next->ip == IP)
        {
            growBoxWaterPrio *toBeDeletedGrowbox = tempGrowBox->next;
            tempGrowBox->next = tempGrowBox->next->next;
            delete toBeDeletedGrowbox;
            updateValveState(checkIfHighestPriority());
            return 1;
        }
        tempGrowBox = tempGrowBox->next;
    }
    return -1;
}

/*There are a total of 6 water levels
(For values inbetween mentioned ranges the water level will stay at current level, as to not quickly change when obtaining water)
0   0-5% 
1   10-20% 
2   25-40% 
3   45-60% 
4   65-80% 
5   85-95%
6   100%+
*/
void WaterSystemGrowBox::updateThisWaterLevel(int moistureMeasurement)
{
    int prevWaterLevel = thisGrowBox->waterLevel;
    if(moistureMeasurement < REQ_WATER_LEVEL(5))
    {
        thisGrowBox->waterLevel = 0;
    } else if (moistureMeasurement > REQ_WATER_LEVEL(10) || moistureMeasurement < REQ_WATER_LEVEL(20))
    {
        thisGrowBox->waterLevel = 1;
    } else if(moistureMeasurement > REQ_WATER_LEVEL(25) || moistureMeasurement < REQ_WATER_LEVEL(40))
    {
        thisGrowBox->waterLevel = 2;
    } else if(moistureMeasurement > REQ_WATER_LEVEL(45) || moistureMeasurement < REQ_WATER_LEVEL(60))
    {
        thisGrowBox->waterLevel = 3;
    } else if(moistureMeasurement > REQ_WATER_LEVEL(65) || moistureMeasurement < REQ_WATER_LEVEL(80))
    {
        thisGrowBox->waterLevel = 4;
    } else if(moistureMeasurement > REQ_WATER_LEVEL(85) || moistureMeasurement < REQ_WATER_LEVEL(95))
    {
        thisGrowBox->waterLevel = 5;
    } else if(moistureMeasurement >= REQ_WATER_LEVEL(100))
    {
        thisGrowBox->waterLevel = 6;
    }
    if(prevWaterLevel != thisGrowBox->waterLevel) {
      updateValveState(checkIfHighestPriority());
    }
}

void WaterSystemGrowBox::saveWaterPrio(int moistureMeasurement, int newPrio)
{
  updateThisWaterLevel(moistureMeasurement);
  thisGrowBox->priority = newPrio;
  updateValveState(checkIfHighestPriority());
}

void WaterSystemGrowBox::sendValveState(int state)
{
  Serial2.println(ISWATERING + state + ENDCHAR);
}

void WaterSystemGrowBox::updateValveState(int isHighestPrio)
{
    if(isHighestPrio != valveState) {
      valveState = isHighestPrio;
      sendValveState(isHighestPrio);
    }
}
