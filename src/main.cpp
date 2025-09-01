#include <Arduino.h>

#define FLOW_SENSOR_PIN 2
#define PULSES_PER_LITER 450.0

volatile uint32_t pulseCount = 0;
volatile float flowRate = 0.0; // Liter pro Minute
volatile float totalLiters = 0.0;

// Variable für die aktuelle Durchflussrate (wird in loop() ausgegeben)
float aktuelleDurchflussrate = 0.0;
float aktuelleGesamtmenge = 0.0;

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR flowSensorISR() {
    pulseCount++;
}

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    flowRate = (pulseCount / PULSES_PER_LITER) * 60.0;
    totalLiters += (pulseCount / PULSES_PER_LITER);
    aktuelleDurchflussrate = flowRate;
    aktuelleGesamtmenge = totalLiters;
    pulseCount = 0;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
    Serial.begin(9600);
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), flowSensorISR, FALLING);

    // Hardware-Timer für 1 Sekunde (1 Hz) konfigurieren
    timer = timerBegin(0, 80, true); // Timer 0, Prescaler 80 (1us Takt)
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true); // 1.000.000us = 1s
    timerAlarmEnable(timer);
}

void loop() {
    // Werte ausgeben
    Serial.print("Durchflussrate: ");
    Serial.print(aktuelleDurchflussrate, 2);
    Serial.print(" L/min, Gesamt: ");
    Serial.print(aktuelleGesamtmenge, 2);
    Serial.println(" L");
    delay(1000); // Ausgabe jede Sekunde
}

