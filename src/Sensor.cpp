#include <Wire.h>
#include <VL53L0X.h>
#include <Arduino.h>
#include "Sensor.h"
#include "Config.h"

VL53L0X sensor;

int nextExecution = 0;

void Sensorsetup()
{
  Wire.begin();

  if (!sensor.init())
  {
    Serial.println("[Sensor.cpp] Failed to initialize VL53L0X!");
    if (RESTART_ON_FAIL)
    {
      ESP.restart();
    }
    else
    {
      ESP.deepSleep(0);
    }
  }

  sensor.startContinuous(); // Sensor reads continuously in the background

  Serial.println("[Sensor.cpp] Sensor is ready!");
}

void Sensorloop()
{
  if (millis() < nextExecution)
  {
    return;
  }
  nextExecution = millis() + SENSOR_MEASUREMENT_DELAY;

  int distance = sensor.readRangeContinuousMillimeters(); // Get the most recent reading

  Serial.print("[Sensor.cpp] Distance: ");
  if (sensor.timeoutOccurred() || distance > 1300)
  {
    Serial.println(1300);
  }
  else
  {
    Serial.println(distance);
  }
}