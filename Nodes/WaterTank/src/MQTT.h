#ifndef MQTT_H
#define MQTT_H
#include <EspMQTTClient.h>

class MQTTHandler
{
    private:
    std::shared_ptr<EspMQTTClient> client;
    public:
    MQTTHandler();
    void sendWaterTankData(int waterLevel);
    void mqttLoop();
};

#endif