#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Servoengine.h"
#include "Sensor.h"
#include "util/Config.h"

// #include "util/Logger.h"

// Logger logger;

bool functionCalled = false;

void setup()
{
  // logger.setup(BAUD_RATE);
  // logger.debug("Debug logging is enabled.");

  Serial.begin(BAUD_RATE);
  Servosetup();
  Sensorsetup();
}

void loop()
{
  Servoloop();

  // Es wird nur im inneren gemessen wenn der Deckel zu ist
  if (!lidOpen && !functionCalled)
  {
    measureInternalDistance();
    functionCalled = true;
  }
  if (lidOpen)
  {
    functionCalled = false;
  }
}
