#include "Sensors.h"
#include <LiquidCrystal.h>

const int rs = 49, en = 45, d4 = 46, d5 = 33, d6 = 44, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void SensorShield::Setup()
{
  humidity.Setup();
  temperature.Setup();
  ldr.Setup();
  soilMoisture.Setup();
}

int SensorShield::ReadButton()
{
  return reading = digitalRead(btn1);
}

void SensorShield::SetupLCD()
{

  lcd.begin(16, 2);
  lcd.clear();
}

int SensorShield::Mode()
{
  ReadButton();
  if (reading != lastBtn1State)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != btn1State)
    {
      btn1State = reading;
      // Serial.println("Kurwa");

      if (btn1State == LOW)
      {
        mode++;
        if (mode == 6)
          mode = 0;
      }
    }
  }
  lastBtn1State = reading;
  return mode;
}

void SensorShield::ShowProgram()
{
  currentMillisProgram = millis();

  if (currentMillisProgram - prevMillisProgram >= intervalProgram)
  {
    prevMillisProgram = currentMillisProgram;

    if (programValue == 0)
    {
      lcd.clear();
      lcd.print("P Duration:");
      lcd.setCursor(0, 1);
      lcd.println(newProgram.GetDuration());
    }
    else if (programValue == 1)
    {
      lcd.clear();
      lcd.print("P Temperature:");
      lcd.setCursor(0, 1);
      lcd.println(newProgram.GetTemperature());
    }
    else if (programValue == 2)
    {
      lcd.clear();
      lcd.print("P Humidity:");
      lcd.setCursor(0, 1);
      lcd.println(newProgram.GetHumidity());
    }
    else if (programValue == 3)
    {
      lcd.clear();
      lcd.print("P Soil Moisture:");
      lcd.setCursor(0, 1);
      lcd.println(newProgram.GetSoilMoisture());
    }
    else if (programValue == 4)
    {
      lcd.clear();
      lcd.print("P Priority:");
      lcd.setCursor(0, 1);
      lcd.println(newProgram.GetPriority());
    }
    else if (programValue == 5)
    {
      lcd.clear();
      lcd.print("P LightCycle:");
      lcd.setCursor(0, 1);
      lcd.println(newProgram.GetLightCycle());
    }

    if (programValue < 6)
      programValue++;
    else if (programValue >= 6)
      programValue = 0;
  }
}

void SensorShield::ShowData(int Mode)
{
  switch (Mode)
  {
    case 1:
      {
        programValue = 0;
        String humMsg = "HUM " + String(humidity.GetHumidity()) + "%";
        lcd.clear();
        lcd.print(humMsg);
      }
      break;
    case 2:
      {
        String ldrMsg = "LUX " + String(ldr.GetLDR());
        lcd.clear();
        lcd.print(ldrMsg);
      }
      break;
    case 3:
      {
        String tempMsg = "TEMP " + String(temperature.GetTemperature());
        lcd.clear();
        lcd.print(tempMsg);
      }
      break;
    case 4:
      lcd.clear();
      lcd.print("Moisture Sensor Value:");
      lcd.setCursor(0, 1);
      lcd.println(soilMoisture.GetSoilMoisture());
      break;
    default:
      lcd.clear();
      lcd.print("Press button to");
      lcd.setCursor(0, 1);
      lcd.print("change mode");
      break;
    case 5:
      ShowProgram();
      break;
  }
}

void SensorShield::SendData()
{
  if (programAssigned == true)
  {
    String message = "$SendData:" + String(humidity.GetHumidity()) + ':' + String(ldr.GetLDR()) + ':' + String(temperature.GetTemperature()) + ':' + String(soilMoisture.GetSoilMoisture()) + '#';
    Serial2.println(message);
    Serial.println(message);
  }
  else if (programAssigned == false)
  {
    String message = "$SendData:" + String(0) + ':' + String(0) + ':' + String(0) + ':' + String(0) + '#';
    Serial2.println(message);
    Serial.println(message);
  }

  //Serial.println(message);
}

void SensorShield::CheckReadings()
{
  if (programAssigned == true)
  {
    /*-------------------------------------Heater Setting-------------------------------------*/
    if (temperature.GetTemperature() < newProgram.GetTemperature())
    {
      digitalWrite(ledY, HIGH);
    }
    else if (temperature.GetTemperature() > newProgram.GetTemperature())
    {
      digitalWrite(ledY, LOW);
    }

    /*-------------------------------------Fan Setting-----------------------------------------*/
    if (humidity.GetHumidity() > newProgram.GetHumidity())
    {
      digitalWrite(ledG, HIGH);
    }
    else if (humidity.GetHumidity() < newProgram.GetHumidity())
    {
      digitalWrite(ledG, LOW);
    }

    /*-------------------------------------Water Setting----------------------------------------*/
    /* if (soilMoisture.GetSoilMoisture() < newProgram.GetSoilMoisture())
      {
       Serial2.println("$Water:" + String(soilMoisture.GetSoilMoisture()) + ':' + String(newProgram.GetPriority()) + '#');
      }*/

    /*-------------------------------------Lighting Setting-------------------------------------*/
    float lightIntensity = newProgram.GetLightIntensity();
    if (ledState == HIGH)
    {
      if (ldr.GetLDR() < lightIntensity)
      {
        lightLevel++;
        if (lightLevel > 100)
          lightLevel = 100;
      }
      else if (ldr.GetLDR() > lightIntensity)
      {
        lightLevel--;
        if (lightLevel < 0)
          lightLevel = 0;
      }
      analogWrite(ledR, lightLevel);
    }
    if (millis() - prevMillis >= newProgram.GetLightCycle() * HOURINMILLIS)
    {
      prevMillis = millis();

      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
        digitalWrite(ledR, ledState);
      }


    }
  }
}
