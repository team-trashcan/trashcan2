#include <Wire.h>
#include <VL53L0X.h>
#include <Arduino.h>

#include "Sensor.h"

VL53L0X sensor;

// // wartezeit zwischen Messungen
// int delayMs = 2*60*1000;

// Testzeit zwischen Messungen
int delayMs = 1000;
int nextExecution = 0;

void Sensorsetup() {
  // This is important - dunno why but it breaks if not present
  Wire.begin();

  // Initialize sensor
  if (!sensor.init()) {
    Serial.println("Failed to initialize VL53L0X!");
    while (1);
  }

  // timeout for waiting on the sensor to return something
  sensor.setTimeout(100);
  sensor.startContinuous();

  Serial.println("Sensor is ready!");
}

void Sensorloop() {
  if (millis() < nextExecution) {
    return;
  }
  nextExecution = millis() + delayMs;

  int distance = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred() || distance > 1300) {
    Serial.println(1300);
    return;
  }
    Serial.println(distance);
}