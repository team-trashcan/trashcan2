#include <ESP32Servo.h>
#include <Arduino.h>
#include "Servoengine.h"
#include "Sensor.h"
#include "util/Config.h"
// #include "util/Logger.h"

long distance, averageDistance, distanceArray[3];
int nextExecutionServo = 0;
int i = 0;
bool lidOpen = false;

Servo servoMotor;
// extern Logger logger;

void Servosetup()
{
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servoMotor.attach(SERVO_PIN);
  servoMotor.write(CLOSE_ANGLE);
  // servoMotor.detach();
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
  if (millis() >= nextExecutionServo)
  {
    // read measure
    // Serial.println("---servo check---");

    nextExecutionServo = millis() + 1000;
    distance = readDistance();
    // Serial.print("distance: ");
    // Serial.println(distance);
  }
  else
  {
    return;
  }

  // Control the servo based on the averaged distance
  if (distance <= DISTANCE_THRESHOLD)
  {
    // Object is close enough, open lid if closed
    if (!lidOpen)
    {
      Serial.println("open lid");
      // servoMotor.attach(SERVO_PIN);
      servoMotor.write(OPEN_ANGLE);
      // servoMotor.detach(); // Detach the servo to save power when not in use

      lidOpen = true;
      nextExecutionServo += LID_HOLD_OPEN_DELAY; // Delay next execution so the lid stays open for some time
    }
  }
  else
  {
    // Object is far away, close lid if open
    if (lidOpen)
    {
      Serial.println("close lid");
      // servoMotor.attach(SERVO_PIN);
      servoMotor.write(CLOSE_ANGLE);
      // servoMotor.detach(); // Detach the servo to save power when not in use
      lidOpen = false;
    }
  }
}
