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

const char *ssid = SSID_NAME;
const char *password = SSID_PASSWORD;

// Your Domain name with URL path or IP address with path
const char *serverName = "http://192.168.1.106:1880/update-sensor";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

// hier den HTTP Client einbauen, da nur diese werte gesendet werden müssen ...

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

  // Setup WIFI
  SetupWIFI();
}

void SetupWIFI()
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    // delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void HTTPClientLoop()
{
  // Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // If you need Node-RED/server authentication, insert user and password below
      // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      // If you need an HTTP request with a content type: application/json, use the following:
      // http.addHeader("Content-Type", "application/json");
      // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      // http.addHeader("Content-Type", "text/plain");
      // int httpResponseCode = http.POST("Hello, World!");

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void Sensorloop()
{
  if (millis() < nextExecutionSensor)
  {
    return;
  }
  nextExecutionSensor = millis() + SENSOR_MEASUREMENT_DELAY;

  distanceInTrashCan = sensor.readRangeContinuousMillimeters(); // Get the most recent reading

  Serial.println("Distance in the trashcan: ");

  if (sensor.timeoutOccurred() || distanceInTrashCan > 1300)
  {
    Serial.println("Distance out of range");
    return;
    // logger.debug("Distance: ", 1300);
  }
  else
  {
    Serial.println(distanceInTrashCan);
    // Send information to Backend/ Server ohne Auswertung - dies geschieht im Backend dann;
    // logger.debug("Distance: ", distance);
  }

  // TODO: Do something with measured distance - send to frontend etc.
}