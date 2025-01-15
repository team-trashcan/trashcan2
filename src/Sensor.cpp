#include <Wire.h>
#include <VL53L0X.h>
#include <Arduino.h>
#include "Sensor.h"
#include "util/Config.h"
// #include "util/Logger.h"

VL53L0X sensor;
// extern Logger logger;

unsigned long nextExecutionSensor = 0;

void Sensorsetup()
{
  Wire.begin();

  if (!sensor.init())
  {
    // logger.error("Failed to initialize VL53L0X!");
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
  Serial.println("Sensor start reading distance");

  // logger.info("VL53L0X sensor is ready!");
}

void Sensorloop()
{
  if (millis() < nextExecutionSensor)
  {
    return;
  }
  nextExecutionSensor = millis() + SENSOR_MEASUREMENT_DELAY;

  int distance = sensor.readRangeContinuousMillimeters(); // Get the most recent reading

  Serial.println("Distance in the trashcan: ");
  Serial.println(distance);

  if (sensor.timeoutOccurred() || distance > 1300)
  {
    return;
    // logger.debug("Distance: ", 1300);
  }
  else
  {
    // logger.debug("Distance: ", distance);
  }

  // TODO: Do something with measured distance - send to frontend etc.
}