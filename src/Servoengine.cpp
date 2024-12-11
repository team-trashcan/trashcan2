/*
  This is an Arduino code for a smart trash can that automatically opens its lid when an object comes within 20cm of it.

  This Arduino code controls an SG90 servo motor based on readings from an ultrasonic distance sensor.
  The distance is measured three times, and the average is computed. If the averaged distance is less
  than or equal to 20, the servo rotates to an open angle of 180 degrees; otherwise, it returns to a
  closed position of 0 degrees after a one-second delay.

  Board: Arduino Uno R4 (or R3)
  Component: Servo motor(SG90) and Ultrasonic distance Sensor(HC-SR04)
*/

#include <ESP32Servo.h>
#include <Arduino.h>

#include "Servoengine.h"

// Set up the ultrasonic sensor parameters
const int trigPin = 5;
const int echoPin = 6;
long distance, averageDistance;
long averDist[3];

// Distance threshold in centimeters
const int distanceThreshold = 20;

#define SERVO_PIN 26 // ESP32 pin GPIO26 connected to servo motor

Servo servoMotor;

void Servosetup()
{
  // Configure the trigger and echo pins of the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servoMotor.attach(SERVO_PIN); // attaches the servo on ESP32 pin
  // servoMotor.write(closeAngle);
  // delay(100);
  // servo.detach();  // Detach the servo to save power when not in use
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
  for (int i = 0; i <= 2; i++)
  {
    distance = readDistance();
    averDist[i] = distance;
    delay(10);
  }

  // Calculate the average distance
  averageDistance = (averDist[0] + averDist[1] + averDist[2]) / 3;
  Serial.println(averageDistance);

  // Control the servo based on the averaged distance
  if (averageDistance <= distanceThreshold)
  {
    // rotates from 0 degrees to 180 degrees
    for (int pos = 0; pos <= 180; pos += 1)
    {
      // in steps of 1 degree
      servoMotor.write(pos);
      delay(15); // waits 15ms to reach the position
    } // Reattach the servo before sending a command
    delay(1);
    // rotates from 180 degrees to 0 degrees
    for (int pos = 180; pos >= 0; pos -= 1)
    {
      servoMotor.write(pos);
      delay(15); // waits 15ms to reach the position // Rotate the servo to the open position
      delay(2000);
    }
  }
  else
  {
    // rotates from 180 degrees to 0 degrees
    for (int pos = 180; pos >= 0; pos -= 1)
    {
      servoMotor.write(pos);
      delay(15); // waits 15ms to reach the position  // Rotate the servo back to the closed position
      delay(500);
      servoMotor.detach(); // Detach the servo to save power when not in use
    }
  }
}