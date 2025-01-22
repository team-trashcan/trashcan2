#ifndef CONFIG_H
#define CONFIG_H

// Preprocessor directive declarations available in all modules
// These get replaced by the preprocessor (before being uploaded) so there's no extra memory usage,
// but their values can't be changed afterwards

#define BAUD_RATE 9600
#define RESTART_ON_FAIL true       // Set to false to disable restart on critical error - the program will shutdown instead
#define DEBUG_LOGGING_ENABLED true // Set to true to enable debug logging

#define TRIGGER_PIN 32
#define ECHO_PIN 33
#define SERVO_PIN 26

#define SENSOR_MEASUREMENT_DELAY 1000 // Time between sensor measurements in milliseconds; Prod: 2*60*1000
#define DISTANCE_THRESHOLD 20         // Distance of detected object for the lid to open, in centimeters
#define OPEN_ANGLE 135                // Lid opened angle, in degree
#define CLOSE_ANGLE 0                 // Lid closed angle, in degree
#define LID_HOLD_OPEN_DELAY 3000      // Delay in milliseconds before lid sensor measures again after opening
#define DISTANCE_CUTOFF 1300          // Maximal distance the sensor is allowed to measure (1300 is hard max)

#define BASE_URL "http://api.wurstkatze.eu/v1" // The base URL where the trashcan sends their data to
// #define WIFI_DELAY 5000                        // Time between sending measurement data over WiFi (default 10 minutes: 600000)
#define RECONNECT_ON_DISCONNECT true // Set to false to not attempt reconnecting if WiFi connection is lost

#endif