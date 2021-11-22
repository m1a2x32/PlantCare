#include "Communication.h"

Communication::Communication(IPAddress serverIP, std::shared_ptr<WaterTank> waterTank, char *WiFiSSID, char *WiFiPass)
{
    Serial.println("begin");
    ServerIp = serverIP;
    this->waterTank = waterTank;
    WiFi.begin(WiFiSSID, WiFiPass);
}

int Communication::ConnectWifi()
{
    while(WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    return 0;
}

void Communication::ConnectToServer(int udpPort)
{
    // Connecting to WiFi...
    //Serial.print("Connecting to ");
    //Serial.print(WIFI_SSID);

    // Loop continuously while WiFi is not connected
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("P");
    }

    // Connected to WiFi
    //Serial.println();
    //Serial.print("Connected! IP address: ");
    localIP = WiFi.localIP();
    //Serial.println(localIP);

    // Announce itself on connection to server
    String anounceMsg = WT_ANNOUNCE + WiFi.localIP().toString() + ENDCHAR;
    sendMessageServer(anounceMsg);

    // Begin listening to UDP port
    UDP.begin(udpPort);
    //Serial.print("Listening on UDP port ");
    //Serial.println(UDP_PORT);
}

void Communication::sendMessageServer(String reply)
// The server will send the address of other nodes to whoever it connects to.
{
    char sendMsg[255];
    reply.toCharArray(sendMsg, reply.length() + 1);
    UDP.beginPacket(ServerIp, 4210);
    int i = 0;
    while (sendMsg[i] != '\0')
    {
        UDP.write((uint8_t)sendMsg[i++]);
    }
    UDP.endPacket();
}

void Communication::sendMessageBack(String message)
{
    char sendMsg[255];
    message.toCharArray(sendMsg, message.length() + 1);
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    int i = 0;
    while(sendMsg[i] != '\0')
    {
        UDP.write((uint8_t)sendMsg[i++]);
    }
    UDP.endPacket();
}

void Communication::listenUDP()
{
    if(UDP.parsePacket())
    {
        char packetBuffer[MAX_MESSAGE_LENGTH];
        UDP.read(packetBuffer, MAX_MESSAGE_LENGTH);
        String message = String(packetBuffer);
        message.remove(message.indexOf(ENDCHAR), 1);
        processMessage(message);
        UDP.flush();
    }
}

void Communication::processMessage(String message)
{
    String identifier = message.substring(message.indexOf(START_CHAR) + 1, message.indexOf(SEPERATOR));

    if (identifier == WT_ANNOUNCE)
    {
        processWTAnnounceReply(message);
    } else if (identifier == WATER_VALVE)
    {
        processWaterValve(message);
    } else if (identifier == WATER_LEAK)
    {
        processWaterLeakReply(message);
    }
}

void Communication::waterTankAnnounce()
{
    String message = WT_ANNOUNCE + localIP + ENDCHAR;
    sendMessageServer(message);
}

void Communication::processWTAnnounceReply(String message)
{
    String errorMessage = message.substring(message.indexOf(SEPERATOR) + 1, message.indexOf(ENDCHAR));
    int errorCode = errorMessage.toInt();
    switch(errorCode)
    {
        case 0:
            //Communication succesfull
            break;
        case 1:
            //IP already exists,
            Serial.println("IP already exists at server");
            break;
        case 2:
            //Message could not be read correctly, resend command
            waterTankAnnounce();
            break;
        default:
            //invalid code
            Serial.println("Invalid Watertank reply");
            break;
    }
}

void Communication::processWaterValve(String message)
{
    int valveValue = message.toInt();
    switch (valveValue)
    {
    case 0:
        //close valve
        waterTank->setRes(1);
        break;
    case 1:
        waterTank->setRes(5);
        //open valve
        break;
    default:
        Serial.println("Invaled WaterValve message");
        break;
    }

    String replyMessage;
    switch(valveValue)
    {
        case 0:
            //Communication succesfull
            break;
        case 1:
            replyMessage = WATER_VALVE + waterTank->isWaterAvailable() + ENDCHAR;
            sendMessageBack(replyMessage);
            break;
        default:
        
            //invalid value
            break;
    }
}

void Communication::processWaterLeakReply(String message)
{
    //Have to figure out how to detect a water leak
}

void Communication::waterLeak()
{
    //Have to figure out how to detect a water leak
}

void Communication::waterTankData()
{
    String message = WATER_DATA + String(waterTank->currentWaterLevel(), 3) + ENDCHAR;
    sendMessageServer(message);
}