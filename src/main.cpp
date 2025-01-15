#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Servoengine.h"
#include "Sensor.h"
#include "util/Config.h"

// #include "util/Logger.h"

// ############################################################################################################
// ################################################## Setup ###################################################
// ############################################################################################################

// Logger logger;

void setup()
{
  // logger.setup(BAUD_RATE);
  // logger.debug("Debug logging is enabled.");

  Serial.begin(BAUD_RATE);
  Servosetup();
  Sensorsetup();
}

// #######################################################################################################
// ################################################# Loop ################################################
// #######################################################################################################

void loop()
{
  // Auf rufen des Servollops
  Servoloop();

  // es wird nur im inneren gemessen wenn der Deckel zu ist
  if (!lidOpen)
  {
    Sensorloop();
  }
}
