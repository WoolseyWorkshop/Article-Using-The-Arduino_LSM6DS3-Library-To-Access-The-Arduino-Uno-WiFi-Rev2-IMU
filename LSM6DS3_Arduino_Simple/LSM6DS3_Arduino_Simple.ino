// LSM6DS3_Arduino_Simple - LSM6DS3_Arduino_Simple.ino
//
// Description:
// Retrieves motion data from the on-board LSM6DS3 IMU of the Arduino Uno WiFi
// Rev2 using the Arduino_LSM6DS3 library and displays readings in the Serial
// Monitor.
//
// Created by John Woolsey on 01/28/2020.
// Copyright (c) 2019 Woolsey Workshop.  All rights reserved.


// Includes
#include <Arduino_LSM6DS3.h>


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
}


void loop() {
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
   delay(1000);  // wait one second between readings
}
