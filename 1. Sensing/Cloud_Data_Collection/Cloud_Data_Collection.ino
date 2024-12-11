#include <WiFi.h>
#include <Wire.h>
#include <MPU9250_asukiaaa.h>
#include <InfluxDbClient.h>

// WiFi credentials
#define WIFI_SSID "WiFi Name"
#define WIFI_PASSWORD "WiFi Password"

// InfluxDB configuration
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "TOKEN"
#define INFLUXDB_ORG "ORG ID"
#define INFLUXDB_BUCKET "SIOT_Test"

// I2C pins for ESP32 DevKitV1
#define SDA_PIN 21
#define SCL_PIN 22

// Initialize sensor and InfluxDB client
MPU9250_asukiaaa mySensor;
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensor("gyro_status");

// Timing variables for 10Hz data collection
unsigned long previousMillis = 0;
const long interval = 100; // 100 milliseconds for 10Hz --> This did not work

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize the sensor
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  Serial.println("MPU9250 initialized.");

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");

  // Disable SSL verification if needed
  client.setInsecure();

  // Validate InfluxDB connection
  if (!client.validateConnection()) {
    Serial.printf("InfluxDB connection failed: %s\n", client.getLastErrorMessage().c_str());
  } else {
    Serial.println("InfluxDB connected successfully.");
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Update sensor data
    mySensor.accelUpdate();
    mySensor.gyroUpdate();

    // Calculate roll and pitch
    float roll = atan2(mySensor.accelY(), mySensor.accelZ()) * 180.0 / PI; // Convert to degrees
    float pitch = atan2(-mySensor.accelX(), sqrt(pow(mySensor.accelY(), 2) + pow(mySensor.accelZ(), 2))) * 180.0 / PI; // Convert to degrees

    // Print values to serial monitor
    Serial.printf("AccelX: %.2f, AccelY: %.2f, AccelZ: %.2f\n", mySensor.accelX(), mySensor.accelY(), mySensor.accelZ());
    Serial.printf("GyroX: %.2f, GyroY: %.2f, GyroZ: %.2f\n", mySensor.gyroX(), mySensor.gyroY(), mySensor.gyroZ());
    Serial.printf("Roll: %.2f, Pitch: %.2f\n", roll, pitch);

    // Prepare data point
    sensor.clearFields();
    sensor.addField("accelX", mySensor.accelX());
    sensor.addField("accelY", mySensor.accelY());
    sensor.addField("accelZ", mySensor.accelZ());
    sensor.addField("gyroX", mySensor.gyroX());
    sensor.addField("gyroY", mySensor.gyroY());
    sensor.addField("gyroZ", mySensor.gyroZ());
    sensor.addField("roll", roll);
    sensor.addField("pitch", pitch);

    // Send data to InfluxDB
    if (client.writePoint(sensor)) {
      Serial.println("Data sent to InfluxDB");
    } else {
      Serial.printf("Failed to send data: %s\n", client.getLastErrorMessage().c_str());
    }
  }
}
