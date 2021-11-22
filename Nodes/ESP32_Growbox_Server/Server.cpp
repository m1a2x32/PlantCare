#include "Server.h"

GrowboxServer::GrowboxServer(IPAddress &broadcastAddr, WiFiUDP &UDP) : broadcastAddr(broadcastAddr), UDP(UDP)
{
  boxListNew = NULL;
  numberOfBoxes = 0;
}

void GrowboxServer::Create()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connecting to wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  setServerIP(WiFi.localIP());
  UDP.begin(UDP_PORT);
  Serial.println("Creation of Server completed!");
}
void GrowboxServer::setServerIP(IPAddress ip)
{
  SERVERIP = ip;
}
void GrowboxServer::setWatertankIP(IPAddress ip)
{
  waterTankIP = ip;
  Serial.print("WaterTank IP: ");
  Serial.println(waterTankIP);
}
void GrowboxServer::waterLeak()
{
  Serial.println("in Water leak function");

  //need to be implemented. maybe send leak warning to dashboard
}
void GrowboxServer::setWaterTankData(String Data)
{
  waterTankData = Data;
}

void GrowboxServer::addBox(IPAddress IP)
{
  growBox *temp = boxListNew;
  if (temp == NULL)
  {
    Serial.println("here");
    boxListNew = new growBox;
    Serial.println("here1");
    boxListNew->ip = IP;
    //Serial.println("here2");
  } else if (temp->ip == IP) {
    //send all saved ip addresses to IP
  } else
  {
    bool checkIP = false;
    while (temp->next != NULL)
    {
      if (IP == temp->ip)
      {
        //send all saved ip addresses to IP
        broadcastMessage(IP, "$Announce:01#");
        checkIP = true;
        return;
      }
      temp = temp->next;
    }
    //Serial.println("here1");
    if (checkIP == false)
    {
      temp->next = new growBox;
      temp = temp->next;
      temp->ip = IP;
    }
    //Serial.println("here2");
    //send all saved ip addresses to IP
    //send IP to all saved ip addressses
  }
  growBox *ipData = boxListNew;

  while (ipData != NULL)
  {
    //String existingIP = String(temp->ip);
    Serial.println("spam");
    broadcastMessage(IP, "$AddNode:" + IpAddress2String(ipData->ip) + ENDCHAR);
    ipData = ipData->next;
    //Serial.println("spam");
  }
}
/*void GrowboxServer::addBox(IPAddress IP)
  {
  // growBox *incomingBox = new growBox;
  //  if (incomingBox != NULL)
  // {
    Serial.println(IP.toString());
  //    incomingBox->next = NULL;
    growBox *temp = NULL;
    if (temp == NULL)
    {
      Serial.println("null?");
      temp = new growBox;
      temp->ip = IP;
      //temp->next = NULL;
      // IPAddress bcastAddress(192, 168, 137, 255);
      // broadcastMessage(bcastAddress, "$AddNode:" + IpAddress2String(IP) + ENDCHAR);
    }
    else
    {
      while (temp->next != NULL)
      {
        if (temp->ip == IP)
        {
          Serial.println("IP already exists");

          //delete incomingBox;
          return;
        }
        temp = temp->next;
      }
      temp->next = new growBox;
      temp->next->ip = IP;
      Serial.println("check1");
    }
    broadcastMessage(IP, "$Announce:00#");
    Serial.print("registered box IP: ");

    Serial.println(IP);
    Serial.println(boxListNew->ip.toString());

    //Serial.println(temp->ip);

  }*/
//delete incomingBox;
//}
// unsigned long preMillisAck = millis();
//int ack = UDP.parsePacket();
//  char receiveack[255];
/* while (!ack)
  {
   //ack = UDP.parsePacket();
   if (millis() - preMillisAck > 10000)
   {
     preMillisAck = millis();
     Serial.println("timeout waiting for growbox");
     delete incomingBox;
     return;
   }
  }*/
/* int len = UDP.read(receiveack, 255);
  if (len > 0)
  {
   Serial.println("here");
   receiveack[len] = '\0';
  }
  String incomingMsg = String(receiveack);
  if (incomingMsg.indexOf(ADDNODE) == 0)
  {

   incomingMsg.remove(incomingMsg.indexOf(ENDCHAR), 1);
   incomingMsg.remove(incomingMsg.indexOf(ADDNODE), 8);
   if (incomingMsg.indexOf(OK) == -1)
   {
     Serial.println("Error");
     delete incomingBox;
     return;
   }
  }*/



void GrowboxServer::removeNode()
{
  // Store head node
  growBox *temp = boxListNew;
  growBox *prev = NULL;
  IPAddress thisIp = UDP.remoteIP();
  // If head node itself holds
  // the key to be deleted
  if (temp != NULL && temp->ip == thisIp)
  {
    boxListNew = temp->next; // Changed head
    delete temp;            // free old head
    Serial.println("IP removed");
    broadcastMessage(thisIp, "$Remove:00#");
    return;
  }

  // Else Search for the key to be deleted,
  // keep track of the previous node as we
  // need to change 'prev->next' */
  else
  {
    while (temp != NULL && temp->ip != thisIp)
    {
      prev = temp;
      temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
    {
      Serial.println("IP not registered");
      broadcastMessage(thisIp, "$Remove:01#");
      return;
    }
    // Unlink the node from linked list
    prev->next = temp->next;
    Serial.println("IP removed");
    broadcastMessage(thisIp, "$Remove:00#");
    // Free memory
    delete temp;
  }
}
void GrowboxServer::cycleCompleted(String endtime)
{
  growBox *temp = boxListNew;
  IPAddress thisIP = UDP.remoteIP();
  while (temp->ip != thisIP)
  {
    temp = temp->next;
    if (temp == NULL)
    {
      broadcastMessage(thisIP, "$CycleCompleted:01#");
      Serial.println("IP not registered");
      delete temp;
      return;
    }
  }
  delete temp;
  broadcastMessage(thisIP, "$CycleCompleted:00#");
}



void GrowboxServer::broadcastMessage(IPAddress ip, String reply)
// The server will send the address of other nodes to whoever it connects to.
{
  char sendMsg[255];
  reply.toCharArray(sendMsg, reply.length() + 1);
  UDP.beginPacket(ip, 4210);
  int i = 0;
  while (sendMsg[i] != '\0')
  {
    UDP.write((uint8_t)sendMsg[i++]);
  }
  i = 0;
  UDP.endPacket();
}

String GrowboxServer::SplitData(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void GrowboxServer::DistributeData(String data)
{
  dataHumidity = SplitData(data, ':', 1);
  dataLdr = SplitData(data, ':', 2);
  dataTemperature = SplitData(data, ':', 3);
  dataSoilHumidity = SplitData(data, ':', 4);

}
/*void GrowboxServer::sendClimateData(IPAddress ip, String Temp, String soilMoisture, String ldr)
  {
  growBox *temp = boxListNew;
  while (temp->ip != ip)
  {
    temp = temp->next;
    if (temp == NULL)
    {
      broadcastMessage(ip, "Data:01#");
      Serial.println("IP not registered");
      delete temp;
      return;
    }
  }
  temp->temperature = Temp;
  temp->soilHumidity = soilMoisture;
  temp->ldr = ldr;
  delete temp;                         //need to be checked
  broadcastMessage(ip, "Data:00#");
  }*/
void GrowboxServer::pauseProgram(IPAddress ip)
{
  String pauseProgram = PAUSE + ENDCHAR;
  broadcastMessage(ip, pauseProgram);
  unsigned long preMillisAck = millis();
  int ack = UDP.parsePacket();
  char receiveack[255];
  while (!ack)
  {
    if (millis() - preMillisAck > 5000)
    {
      Serial.println("timeout waiting for growbox");
      return;
    }
  }
  int len = UDP.read(receiveack, 255);
  if (len > 0)
  {
    receiveack[len] = '\0';
  }
  String incomingMsg = String(receiveack);
  if (incomingMsg.indexOf(PAUSE) == 0)
  {
    incomingMsg.remove(incomingMsg.indexOf(ENDCHAR), 1);
    incomingMsg.remove(incomingMsg.indexOf(PAUSE), 7);
    if (incomingMsg.indexOf("01") == 0)
    {
      Serial.println("Program paused, no error");
      //send to mqtt need to be implemented
      return;
    }
    else if (incomingMsg.indexOf("02") == 0)
    {
      Serial.println("No program assigned");
      //send to mqtt need to be implemented
      return;
    }
    else if (incomingMsg.indexOf("03") == 0)
    {
      Serial.println("Program already paused");
      //send to mqtt need to be implemented
      return;
    }
  }
}
void GrowboxServer::resumeProgram(IPAddress ip)
{
  String resumeProgram = RESUME + ENDCHAR;
  broadcastMessage(ip, resumeProgram);
  unsigned long preMillisAck = millis();
  int ack = UDP.parsePacket();
  char receiveack[255];
  while (!ack)
  {
    if (millis() - preMillisAck > 5000)
    {
      Serial.println("timeout waiting for growbox");
      return;
    }
  }
  int len = UDP.read(receiveack, 255);
  if (len > 0)
  {
    receiveack[len] = '\0';
  }
  String incomingMsg = String(receiveack);
  if (incomingMsg.indexOf(RESUME) == 0)
  {
    incomingMsg.remove(incomingMsg.indexOf(ENDCHAR), 1);
    incomingMsg.remove(incomingMsg.indexOf(RESUME), 7);
    if (incomingMsg.indexOf("01") == 0)
    {
      Serial.println("Program resumed, no error");
      //send to mqtt need to be implemented
      return;
    }
    else if (incomingMsg.indexOf("02") == 0)
    {
      Serial.println("No program assigned");
      //send to mqtt need to be implemented
      return;
    }
    else if (incomingMsg.indexOf("03") == 0)
    {
      Serial.println("Program already running");
      //send to mqtt need to be implemented
      return;
    }
  }
}
void GrowboxServer::sensorError(String sensorType, String errorType)
{
  IPAddress thisIP = UDP.remoteIP();
  growBox *temp = boxListNew;
  while (temp->ip != thisIP)
  {
    temp = temp->next;
    if (temp == NULL)
    {
      broadcastMessage(thisIP, "SensorError:01#");
      Serial.println("IP not registered");
      delete temp;
      return;
    }
  }
  Serial.println("Sensor error recorded");
  Serial.print("SensorType: ");
  Serial.println(sensorType);
  Serial.print("ErrorType: ");
  Serial.println(errorType);
  //need to be stored somewhere
  delete temp;
  return;
}
void GrowboxServer::GetJsonBuffer(String humidity, String ldr, String temperature, String soilHumidity)
{
  //  char buffer[BUF_LEN];
  StaticJsonDocument<BUF_LEN> doc;
  //  client.loop();
  // delay(1000);
  doc["humidity"] = humidity;
  doc["ldr"] = ldr;
  doc["temperature"] = temperature;
  doc["soilHumidity"] = soilHumidity;
  doc["IP_Addr"] = UDP.remoteIP().toString();

  serializeJson(doc, msgBuffer);
}

void GrowboxServer::replyRequestProgramData()
{
  growBox *temp = boxListNew;
  IPAddress thisIP = UDP.remoteIP();
  while (temp->ip != thisIP)
  {
    temp = temp->next;
    if (temp == NULL)
    {
      broadcastMessage(thisIP, "$ReqProgSpec:00#");
      Serial.println("IP not registered");
      delete temp;
      return;
    }
  }
  String endtime, Temp, soilmoisture, lightintensity, lightcycle;
  endtime = temp->endtime;
  Temp = temp->temperature;
  soilmoisture = temp->soilHumidity;
  lightintensity = temp->ldr;
  lightcycle = temp->lightcycle;
  String reply = REQPROGSPEC + endtime + ":" + Temp + ":" + soilmoisture + ":" + lightintensity + ":" + lightcycle + ENDCHAR;
  broadcastMessage(thisIP, reply);
  return;
}

void GrowboxServer::ReadDashboard(String message)
{
  //message.remove(incomingMsg.indexOf(ENDCHAR), 1);
  if (message.indexOf(ASSIGNPROGRAM) == 0)
  {
    String ipMsg = SplitData(message, ':', 1);
    int ip[4];
    ip[0] = SplitData(ipMsg, '.', 0).toInt();
    ip[1] = SplitData(ipMsg, '.', 1).toInt();
    ip[2] = SplitData(ipMsg, '.', 2).toInt();
    ip[3] = SplitData(ipMsg, '.', 3).toInt();
    IPAddress ipAddr(ip[0], ip[1], ip[2], ip[3]);
    broadcastMessage(ipAddr, message);
  }
}

String GrowboxServer::IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3])  ;
}


String GrowboxServer::getHumid()
{
  return boxListNew->humidity;
}
String GrowboxServer::getldr()
{
  return boxListNew->ldr;
}
String GrowboxServer::getTemp()
{
  return boxListNew->temperature;
}
String GrowboxServer::getsoilHumidity()
{
  return boxListNew->soilHumidity;
}
