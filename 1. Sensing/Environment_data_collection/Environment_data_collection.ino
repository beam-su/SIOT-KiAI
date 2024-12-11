#include <DHT11.h>

// Declare pinout
const int micPin = 2;
DHT11 dht11(3);

void setup() {
    pinMode(micPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    int micOutput = digitalRead(micPin);
    int temperature = 0;
    int humidity = 0;

    int result = dht11.readTemperatureHumidity(temperature, humidity);

    // Format data as JSON
    Serial.print("{");
    Serial.print("\"mic\":");
    Serial.print(micOutput == HIGH ? 0 : 1); // 0 is low
    if (result == 0) {
        Serial.print(",\"temperature\":");
        Serial.print(temperature);
        Serial.print(",\"humidity\":");
        Serial.print(humidity);
    } else {
        Serial.print(",\"error\":\"");
        Serial.print(DHT11::getErrorString(result));
        Serial.print("\"");
    }
    Serial.println("}");

    // Delay for stability
    delay(1000);
}
