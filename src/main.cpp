#include <Arduino.h>


#define NUM_SENSORS 4
const uint8_t FLOW_SENSOR_PINS[NUM_SENSORS] = {2, 3, 4, 5};
#define PULSES_PER_LITER 450.0

volatile uint32_t pulseCount[NUM_SENSORS] = {0};
volatile float flowRate[NUM_SENSORS] = {0.0}; // Liter pro Minute
volatile float totalLiters[NUM_SENSORS] = {0.0};

float aktuelleDurchflussrate[NUM_SENSORS] = {0.0};
float aktuelleGesamtmenge[NUM_SENSORS] = {0.0};

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR flowSensorISR0() { pulseCount[0]++; }
void IRAM_ATTR flowSensorISR1() { pulseCount[1]++; }
void IRAM_ATTR flowSensorISR2() { pulseCount[2]++; }
void IRAM_ATTR flowSensorISR3() { pulseCount[3]++; }

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    for (int i = 0; i < NUM_SENSORS; i++) {
        flowRate[i] = (pulseCount[i] / PULSES_PER_LITER) * 60.0;
        totalLiters[i] += (pulseCount[i] / PULSES_PER_LITER);
        aktuelleDurchflussrate[i] = flowRate[i];
        aktuelleGesamtmenge[i] = totalLiters[i];
        pulseCount[i] = 0;
    }
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < NUM_SENSORS; i++) {
        pinMode(FLOW_SENSOR_PINS[i], INPUT_PULLUP);
    }
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PINS[0]), flowSensorISR0, FALLING);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PINS[1]), flowSensorISR1, FALLING);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PINS[2]), flowSensorISR2, FALLING);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PINS[3]), flowSensorISR3, FALLING);

    // Hardware-Timer für 1 Sekunde (1 Hz) konfigurieren
    timer = timerBegin(0, 80, true); // Timer 0, Prescaler 80 (1us Takt)
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true); // 1.000.000us = 1s
    timerAlarmEnable(timer);
}

void loop() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        Serial.print("Sensor ");
        Serial.print(i+1);
        Serial.print(": Durchflussrate: ");
        Serial.print(aktuelleDurchflussrate[i], 2);
        Serial.print(" L/min, Gesamt: ");
        Serial.print(aktuelleGesamtmenge[i], 2);
        Serial.println(" L");
    }
    Serial.println("----------------------");
    delay(1000); // Ausgabe jede Sekunde
}

