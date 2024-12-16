#include <ESP32Servo.h>
#include <Arduino.h>
#include "Servoengine.h"
#include "Sensor.h"

// Set up the ultrasonic sensor parameters
const int trigPin = 32;
const int echoPin = 33;
long distance, averageDistance;
long averDist[3];

// Distance threshold in centimeters
const int distanceThreshold = 20;

int nextExecution = 0;
int i = 0;

#define SERVO_PIN 26 // ESP32 pin GPIO26 connected to servo motor

// Degrees for close and open Angle
const int openAngle = 135;
const int closeAngle = 0;

Servo servoMotor;

void Servosetup()
{
  // Configure the trigger and echo pins of the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servoMotor.attach(SERVO_PIN); // attaches the servo on ESP32 pin
  servoMotor.write(closeAngle);
  if (millis() < nextExecution)
  {
    return;
  }
  nextExecution = millis() + 10;
  servoMotor.detach(); // Detach the servo to save power when not in use

  Sensorsetup();
}

// Function to read the sensor data and calculate the distance
float readDistance()
{
  // Send a pulse on the trigger pin of the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the pulse width of the echo pin and calculate the distance value
  float distance = pulseIn(echoPin, HIGH) / 58.00; // Formula: (340m/s * 1us) / 2
  return distance;
}

void Servoloop()
{
  // Measure the distance three times
  // for (int i = 0; i <= 2; i++)
  if (millis() >= nextExecution && i <= 2)
  {
    nextExecution = millis() + 10;
    distance = readDistance();
    averDist[i] = distance;
    i++;
  }
  else if (i != 3)
  {
    return;
  }
  i = 0;

  // Calculate the average distance
  averageDistance = (averDist[0] + averDist[1] + averDist[2]) / 3;
  Serial.println(averageDistance);

  // Control the servo based on the averaged distance
  if (averageDistance <= distanceThreshold)
  {
    // open Angle
    servoMotor.write(openAngle);
    if (millis() < nextExecution)
    {
      return;
    }
    nextExecution = millis() + 1000;
    // close Angle after 1000 ms
    servoMotor.write(closeAngle);
    if (millis() < nextExecution)
    {
      return;
    }
    nextExecution = millis() + 200;

    Sensorloop();
  }

  else
  {
    // close Angle
    servoMotor.write(closeAngle);
    if (millis() < nextExecution)
    {
      return;
    }
    nextExecution = millis() + 15;
    servoMotor.detach(); // Detach the servo to save power when not in use
  }
}
