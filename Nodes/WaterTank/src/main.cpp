#include <Arduino.h>
#include "WaterTank.h"
#include "Communication.h"
#include "MQTT.h"

IPAddress serverIP(192, 168, 137, 5);

std::shared_ptr<WaterTank> waterTank;
std::shared_ptr<Communication> com;
std::shared_ptr<MQTTHandler> mqtt;

char ssid[] = "TomaPC";
char ssidPass[] = "12345678";

const int waterTankDataInterval = 5000;

void setup() {
    Serial.begin(115200);
    waterTank = std::make_shared<WaterTank>(MAX_WATERTANK_VALUE);
    com = std::make_shared<Communication>(serverIP, waterTank, ssid, ssidPass);
    com->ConnectWifi();
    mqtt = std::make_shared<MQTTHandler>();
    com->ConnectToServer(4210);
    //com->waterTankAnnounce();
}

void loop() {
    com->listenUDP();
    mqtt->mqttLoop();
    static long long lastWaterLevelUpdate;
    if(millis() > lastWaterLevelUpdate + 500)
    {
        waterTank->getWaterLevel();
        lastWaterLevelUpdate = millis();
    }
    static long long lastMQTTUpdate;
    if(millis() > lastMQTTUpdate + 5000)
    {
        mqtt->sendWaterTankData(waterTank->currentWaterLevel());
        lastMQTTUpdate = millis();
    }

    //test code
    static long long lastMessageSend;
    if(millis() > lastMessageSend + 8575)
    {
        static int toggle = 0;
        if(toggle) {
            waterTank->setRes(5);
            toggle = 0;
        } else{
            waterTank->setRes(1);
            toggle = 1;
        }
        lastMessageSend = millis();
    }

    //actual code
    static double prevWaterTankDataSend = 0;
    static long long lastWaterTankDataSend = 0;
    double waterTankData = waterTank->currentWaterLevel();
    if (waterTankData < .95 * prevWaterTankDataSend || waterTankData > 1.05 * prevWaterTankDataSend || millis() > lastWaterTankDataSend + waterTankDataInterval)
    {
        //com->waterTankData();
        prevWaterTankDataSend = waterTankData;
        lastWaterTankDataSend = millis();
    }
}

void onConnectionEstablished()
{

}