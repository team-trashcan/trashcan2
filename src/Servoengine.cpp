#include <ESP32Servo.h>
#include <Arduino.h>
#include "Servoengine.h"
#include "Sensor.h"
#include "Config.h"

long distance, averageDistance, distanceArray[3];
int nextExecution = 0;
int i = 0;
bool lidOpen = false;

Servo servoMotor;

void Servosetup()
{
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servoMotor.attach(SERVO_PIN);
  servoMotor.write(CLOSE_ANGLE);
  servoMotor.detach(); // Save power when not in use

  Sensorsetup();
}

// Function to read the sensor data and calculate the distance
float readDistance()
{
  // Send a pulse on the trigger pin of the ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  return pulseIn(ECHO_PIN, HIGH) / 58.00; // Formula: (340m/s * 1us) / 2
}

void Servoloop()
{
  // Measure the distance three times
  if (millis() >= nextExecution && i <= 2)
  {
    nextExecution = millis() + 10;
    distance = readDistance();
    distanceArray[i] = distance;
    i++;
  }
  else if (i != 3)
  {
    return;
  }
  i = 0;

  // Calculate the average distance
  averageDistance = (distanceArray[0] + distanceArray[1] + distanceArray[2]) / 3;
  Serial.print("[Servoengine.cpp] Average distance: ");
  Serial.println(averageDistance);

  // Control the servo based on the averaged distance
  if (averageDistance <= DISTANCE_THRESHOLD)
  {
    // Object is close enough, open lid if closed
    if (!lidOpen)
    {
      servoMotor.attach(SERVO_PIN);
      servoMotor.write(OPEN_ANGLE);

      lidOpen = true;
      nextExecution += LID_HOLD_OPEN_DELAY; // Delay next execution so the lid stays open for some time
    }
  }
  else
  {
    // Object is far away, close lid if open
    if (lidOpen)
    {
      servoMotor.attach(SERVO_PIN);
      servoMotor.write(CLOSE_ANGLE);

      lidOpen = false;
    }
  }

  servoMotor.detach(); // Detach the servo to save power when not in use
}
