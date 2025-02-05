#include <Wire.h>
#include <VL53L0X.h>
#include <Arduino.h>
#include "Sensor.h"
#include "util/Config.h"
#include <WiFi.h>
#include <HTTPClient.h>
// #include "util/Logger.h"

VL53L0X sensor;

int distanceInTrashCan;
// extern Logger logger;

unsigned long nextExecutionSensor = 0;
unsigned long lastExecutionWifi = 0;
unsigned long nextExecutionWifi = 0;

// Use the SSID defined in platformio.ini
const char *ssid = SSID_NAME;

void SetupWIFI()
{
  Serial.println("WiFi Connecting...");
  Serial.println(ssid);
  WiFi.begin(ssid, SSID_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    // We can use a delay here, just trap execution until connected
    delay(500);
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void Sensorsetup()
{
  Wire.begin();

  if (!sensor.init())
  {
    // logger.error("Failed to initialize VL53L0X!");
    Serial.println("Failed to initialize VL53L0X!");
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
  Serial.println("VL53L0X sensor is ready!");
  // logger.info("VL53L0X sensor is ready!");

  SetupWIFI();
}

void SendHTTPPost()
{
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;

    Serial.println("Sending HTTP POST request...");
    // Your Domain name with URL path or IP address with path
    http.begin(client, BASE_URL "/trashcan-fill-height");

    // If you need Node-RED/server authentication, insert user and password below
    // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{\"name\":\"1\",\"data\":{\"distance\":\"" + String(distanceInTrashCan) + "\"}}");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi was disconnected");
    if (RECONNECT_ON_DISCONNECT)
    {
      SetupWIFI();
    }
  }
}

void measureInternalDistance()
{
  distanceInTrashCan = sensor.readRangeContinuousMillimeters(); // Get the most recent reading

  if (sensor.timeoutOccurred())
  {
    Serial.println("Sensor timeout occurred");
    return;
  }

  if (distanceInTrashCan > DISTANCE_CUTOFF)
  {
    // Sensor measured far, so trashcan is empty - dont save to batch
    Serial.println("Sensor distance out of range");
    return;
    // logger.debug("Distance: ", 1300);
  }
  else
  {
    // TODO: save this to an array or something, then send over WiFi every 10 minutes in batch, clear array afterwards
    // Send information to Backend/Server ohne Auswertung - dies geschieht im Backend dann;
    Serial.print("Sensor distance: ");
    Serial.println(distanceInTrashCan);
    // logger.debug("Distance: ", distance);
    SendHTTPPost();
  }
}