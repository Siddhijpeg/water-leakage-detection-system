#define BLYNK_PRINT Serial
#include <BlynkSimpleStream.h>

char auth[] = "IfcjqR6kJIqZdVMoXKJwS_j8BktgYC57";  // Replace with your Blynk Auth Token

int LEAKAGE_PIN = A0; // Analog pin to read voltage
int CONTROL_PIN = 3;  // Digital pin to send current
int THRESHOLD = 1000;
bool flag = false;
unsigned long start = 0;
float firstval = 0.0;
int l = 0;

void setup() {
    Serial.begin(9600);  // Start Serial communication for USB
    Blynk.begin(Serial, auth);  // Connect to Blynk using USB

    pinMode(CONTROL_PIN, OUTPUT);
    pinMode(LEAKAGE_PIN, INPUT);
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);

    digitalWrite(CONTROL_PIN, LOW);
    Serial.println("Leakage Detection System Initialized.");
    Serial.println("Waiting for sensor readings...");
}

void loop() {
    Blynk.run();  // Run Blynk
    
    // Send current and read sensor data
    digitalWrite(CONTROL_PIN, HIGH);
    delay(100);
    float sensorValue = analogRead(LEAKAGE_PIN);
    Serial.print("Sensor Voltage: ");
    Serial.println(sensorValue);

    if (sensorValue < THRESHOLD) {
        if (!flag) {
            flag = true;
            start = micros();
            Serial.println("🚨 Leakage Detected!");
            Blynk.logEvent("leak_alert", "🚨 Leakage Detected! Check immediately.");
        }

        if (l == 0) {
            firstval = micros() - start;
        }

        Serial.print("Duration: ");
        Serial.print((micros() - start) / 1000000);
        Serial.println(" seconds");

        if (sensorValue > 600) {
            digitalWrite(13, HIGH);
            digitalWrite(12, LOW);
        } else {
            digitalWrite(12, HIGH);
            digitalWrite(13, LOW);
        }

        Blynk.virtualWrite(V0, sensorValue);  // Send data to Blynk

    } else {
        Serial.println("No Leakage Detected");
        digitalWrite(13, LOW);
        digitalWrite(12, LOW);
        flag = false;
    }

    digitalWrite(CONTROL_PIN, LOW);
    delay(1000);
}
