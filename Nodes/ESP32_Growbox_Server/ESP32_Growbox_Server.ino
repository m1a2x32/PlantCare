#include "Server.h"

IPAddress broadcastAddress(192, 168, 137, 255);
WiFiUDP UDP;
GrowboxServer server(broadcastAddress, UDP);

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

EspMQTTClient client
(
  "test.mosquitto.org", // MQTT Broker
  1883,              // The MQTT port, default to 1883. this line can be omitted
  "Server"     // Client name that uniquely identify your device
);

void setup()
{
  startDisplay();
  server.Create();
  tft.print("IP address: ");
  tft.println();
  tft.print(WiFi.localIP());
  tft.println();
}

void onConnectionEstablished() {
  client.subscribe("growbox/commands", [] (const String & payload)  {
    server.ReadDashboard(payload);
    Serial.println(payload);
  });
}

void loop() {
  client.loop();
  char packet[255];
  int packetSize = UDP.parsePacket();
  if (packetSize)
  {
    tft.setCursor(0, 0);
    tft.fillScreen(ST77XX_BLACK);
    Serial.println("UDP message received!");
    tft.print("UDP message received!");
    tft.println();
    int len = UDP.read(packet, 255);
    if (len > 0)
    {
      packet[len] = '\0';
    }
    String incomingMsg = String(packet);
    Serial.print("Incoming IP: ");
    tft.print("Incoming IP: ");
    tft.println();
    Serial.println(UDP.remoteIP());
    tft.print(UDP.remoteIP());
    tft.println();
    Serial.print("Incoming UDP message is: ");
    tft.print("Incoming UDP message is: ");
    tft.println();
    Serial.println(incomingMsg);
    tft.print(incomingMsg);
    tft.println();
    incomingMsg.remove(incomingMsg.indexOf(ENDCHAR), 1);
    if (incomingMsg.indexOf(ANNOUNCE) == 0)
    { // After growbox announces it self, send add node msg
      IPAddress incomingIP = UDP.remoteIP();
      Serial.println("ANNOUNCE received");
      tft.print("ANNOUNCE received");
      tft.println();
      server.broadcastMessage(broadcastAddress, "$AddNode:" + server.IpAddress2String(incomingIP) + ENDCHAR);
      server.addBox(incomingIP);
    }
    else if (incomingMsg.indexOf(SENDDATA) == 0)
    {
      Serial.println("SENDDATA received");
      tft.print("SENDDATA received");
      tft.println();
      server.DistributeData(incomingMsg);
      server.GetJsonBuffer(server.dataHumidity, server.dataLdr, server.dataTemperature, server.dataSoilHumidity);
      client.publish("growbox/data", server.msgBuffer);
      String ack = "$SendData:00#";
      server.broadcastMessage(UDP.remoteIP(), ack);
    }
    else if (incomingMsg.indexOf(REQPROGSPEC) == 0)
    {
      Serial.println("REQPROGSPEC received");
      tft.print("REQPROGSPEC received");
      tft.println();
      incomingMsg.remove(incomingMsg.indexOf(REQPROGSPEC), 13);
      server.replyRequestProgramData();
    }
    else if (incomingMsg.indexOf(CYCLECOMPLETED) == 0)
    {
      Serial.println("CYCLECOMPLETED received");
      tft.print("CYCLECOMPLETED received");
      tft.println();
      incomingMsg.remove(incomingMsg.indexOf(CYCLECOMPLETED), 16);
      server.cycleCompleted(incomingMsg);
      String reply = CYCLECOMPLETED + UDP.remoteIP() + incomingMsg + ENDCHAR;
      client.publish("GrowboxServer/completed", reply);
    }
    else if (incomingMsg.indexOf(SENSORERROR) == 0)
    {
      Serial.println("SENSORERROR received");
      tft.print("SENSORERROR received");
      tft.println();
      //incomingMsg.remove(incomingMsg.indexOf(SENSORERROR), 13);
      String sensorType = server.SplitData(incomingMsg, ':', 1);
      String errorType = server.SplitData(incomingMsg, ':', 2);
      server.sensorError(sensorType, errorType);
    }
    else if (incomingMsg.indexOf(WATERLEAK) == 0)
    {
      Serial.println("WATERLEAK received");
      tft.print("WATERLEAK received");
      tft.println();
      incomingMsg.remove(incomingMsg.indexOf(WATERLEAK), 11);
      server.waterLeak();
    }
    else if (incomingMsg.indexOf(WATERTANKANNOUNCE) == 0)
    {
      Serial.println("WATERTANKANNOUNCE received");
      tft.print("WATERTANKANNOUNCE received");
      tft.println();
      //incomingMsg.remove(incomingMsg.indexOf(WATERTANKANNOUNCE), 19);
      server.setWatertankIP(UDP.remoteIP());
    }
    else if (incomingMsg.indexOf(WATERTANKDATA) == 0)
    {
      Serial.println("WATERTANKDATA received");
      tft.print("WATERTANKDATA received");
      tft.println();
      // incomingMsg.remove(incomingMsg.indexOf(WATERTANKDATA), 19);
      String waterLevel = server.SplitData(incomingMsg, ':', 1);
      server.setWaterTankData(waterLevel);
    }
  }
}

void startDisplay()
{
  tft.init(240, 240, SPI_MODE2);    // Init ST7789 display 240*240 pixel
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.println("Server Started");
}
