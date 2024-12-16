# Intelligente Mülltonne

This is the Arduino code for a smart trashcan (Intelligente Mülltonne in german) that opens its lid automatically when an object comes within 20cm of it.

The distance is measured three times, and the average is computed. If the averaged distance is less
than or equal to 20, the servo rotates to an open angle of 180 degrees; otherwise, it returns to a
closed position of 0 degrees after a one-second delay.

## Used components

> The following information might change in the process and is not final

- ESP32 board (todo: exact version)
- Servo motor (SG90)
- Ultrasonic distance sensor (HC-SR04)
- Laser distance sensor (ToF VL53L0X)
