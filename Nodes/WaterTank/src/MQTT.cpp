#include "MQTT.h"

MQTTHandler::MQTTHandler(){
    client = std::make_shared<EspMQTTClient>("test.mosquitto.org", 1883, "WaterTank");
}

void MQTTHandler::sendWaterTankData(int waterLevel)
{
    client->publish("growbox/watertank", String(waterLevel));
    Serial.println(waterLevel);
}

void MQTTHandler::mqttLoop()
{
    client->loop();
}
