#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <String.h>
#include <stdint.h>
#include "WaterTank.h"
//#include "Communication.cpp"

//UDP Connection
const String WIFI_SSID = "GrowBox-Server";
const String WIFI_PASS = "12345678";
const String UDP_PORT = "4210";

//Growbox limit
const int MAXGROWBOXES = 10;
const int MAX_MESSAGE_LENGTH = 255;

// COM PROTOCOL MACROS
#define ENDCHAR '#'
#define START_CHAR '$'
#define SEPERATOR ':'
#define ANNOUNCE "$Announce:"
#define ADD_WT "$AddWaterTank:"
#define WT_ANNOUNCE "$WaterTankAnnounce:"
#define WATER_VALVE "$WaterValve:"
#define WATER_LEAK "$WaterLeak:"
#define WATER_DATA "$WaterTankData:"
#define ADDNODE "$AddNode:"
#define SENDDATA "$SendData:"
#define ASSIGNPROGRAM "$Program:"
#define PAUSE "$Pause#"
#define RESUME "$Resume#"

class Communication
{
private:
    // Growbox variables
    int numberOfBoxes;
    // Communication variables
    IPAddress boxList[MAXGROWBOXES];
    IPAddress ServerIp;
    uint16_t serverPort; 
    IPAddress localIP;
    IPAddress drainingGrowBoxIP;
    WiFiUDP UDP;
    std::shared_ptr<WaterTank> waterTank;
    void processMessage(String message);
    void processWTAnnounceReply(String message);
    void processWaterValve(String message);
    void processWaterLeakReply(String message);
    void sendMessageBack(String message);
public:
    Communication(IPAddress ServerIp, std::shared_ptr<WaterTank> waterTank, char *WiFiSSID, char *WiFiPass);
    int ConnectWifi();
    void ConnectToServer(int udpPort);
    void addBox(String ip);
    void sendMessageServer(String reply);

    void listenUDP();
    void waterTankAnnounce();

    void waterLeak();
    void waterTankData();
};

#endif