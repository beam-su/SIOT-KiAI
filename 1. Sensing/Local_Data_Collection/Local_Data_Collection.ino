#include <Wire.h>
#include <MPU9250_asukiaaa.h>
#include <math.h>  // Required for atan2 and sqrt

MPU9250_asukiaaa mySensor;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA = 21, SCL = 22 for ESP32

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();

  Serial.println("MPU9250 initialized.");
}

void loop() {
  // Update accelerometer and gyroscope data
  mySensor.accelUpdate();
  mySensor.gyroUpdate();

  // Calculate Roll and Pitch
  float roll = atan2(mySensor.accelY(), mySensor.accelZ()) * 180 / PI; // Convert to degrees
  float pitch = atan2(-mySensor.accelX(), sqrt(mySensor.accelY() * mySensor.accelY() + mySensor.accelZ() * mySensor.accelZ())) * 180 / PI; // Convert to degrees

  // Print accelerometer, gyroscope, roll, and pitch data in CSV format
  Serial.print(mySensor.accelX());
  Serial.print(",");
  Serial.print(mySensor.accelY());
  Serial.print(",");
  Serial.print(mySensor.accelZ());
  Serial.print(",");
  Serial.print(mySensor.gyroX());
  Serial.print(",");
  Serial.print(mySensor.gyroY());
  Serial.print(",");
  Serial.print(mySensor.gyroZ());
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.println(pitch);

  delay(100);
}
