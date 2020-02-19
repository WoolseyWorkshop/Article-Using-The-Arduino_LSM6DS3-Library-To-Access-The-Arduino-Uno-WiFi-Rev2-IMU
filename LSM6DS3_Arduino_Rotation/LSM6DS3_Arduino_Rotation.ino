// LSM6DS3_Arduino_Rotation - LSM6DS3_Arduino_Rotation.ino
//
// Description:
// Retrieves motion data from the on-board LSM6DS3 IMU of the Arduino Uno WiFi
// Rev2 using the Arduino_LSM6DS3 library and displays rotation angles (roll,
// pitch, and yaw) in the Serial Monitor.
//
// Created by John Woolsey on 01/28/2020.
// Copyright (c) 2019 Woolsey Workshop.  All rights reserved.


// Includes
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>


// Defines
#define SAMPLE_RATE 10  // in Hz


// Constructors
Madgwick filter;  // Madgwick algorithm for roll, pitch, and yaw calculations


void setup() {
   Serial.begin(9600);  // initialize serial bus (Serial Monitor)
   while (!Serial);  // wait for serial initialization
   Serial.print("LSM6DS3 IMU initialization ");
   if (IMU.begin()) {  // initialize IMU
      Serial.println("completed successfully.");
   } else {
      Serial.println("FAILED.");
      IMU.end();
      while (1);
   }
   Serial.println();
   filter.begin(SAMPLE_RATE);  // initialize Madgwick filter
}


void loop() {
   static unsigned long previousTime = millis();
   unsigned long currentTime = millis();
   if (currentTime - previousTime >= 1000/SAMPLE_RATE) {
      // printValues();
      printRotationAngles();
      previousTime = millis();
   }
}


// Prints IMU values.
void printValues() {
   char buffer[8];    // string buffer for use with dtostrf() function
   float ax, ay, az;  // accelerometer values
   float gx, gy, gz;  // gyroscope values

   // Retrieve and print IMU values
   if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()
      && IMU.readAcceleration(ax, ay, az) && IMU.readGyroscope(gx, gy, gz)) {
      Serial.print("ax = ");  Serial.print(dtostrf(ax, 4, 1, buffer));  Serial.print(" g, ");
      Serial.print("ay = ");  Serial.print(dtostrf(ay, 4, 1, buffer));  Serial.print(" g, ");
      Serial.print("az = ");  Serial.print(dtostrf(az, 4, 1, buffer));  Serial.print(" g, ");
      Serial.print("gx = ");  Serial.print(dtostrf(gx, 7, 1, buffer));  Serial.print(" °/s, ");
      Serial.print("gy = ");  Serial.print(dtostrf(gy, 7, 1, buffer));  Serial.print(" °/s, ");
      Serial.print("gz = ");  Serial.print(dtostrf(gz, 7, 1, buffer));  Serial.println(" °/s");
   }
}


// Prints rotation angles (roll, pitch, and yaw) calculated using the
// Madgwick algorithm.
// Note: Yaw is relative, not absolute, based on initial starting position.
// Calculating a true yaw (heading) angle requires an additional data source,
// such as a magnometer.
void printRotationAngles() {
   char buffer[5];    // string buffer for use with dtostrf() function
   float ax, ay, az;  // accelerometer values
   float gx, gy, gz;  // gyroscope values

   if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()
      && IMU.readAcceleration(ax, ay, az) && IMU.readGyroscope(gx, gy, gz)) {
      filter.updateIMU(gx, gy, gz, ax, ay, az);  // update roll, pitch, and yaw values

      // Print rotation angles
      Serial.print("Roll = ");  Serial.print(dtostrf(filter.getRoll(), 4, 0, buffer)); Serial.print(" °, ");
      Serial.print("Pitch = ");  Serial.print(dtostrf(filter.getPitch(), 4, 0, buffer)); Serial.print(" °, ");
      Serial.print("Yaw = ");  Serial.print(dtostrf(filter.getYaw(), 4, 0, buffer)); Serial.println(" °");
   }
}
