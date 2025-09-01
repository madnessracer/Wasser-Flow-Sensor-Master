
#include <Arduino.h>
#include "flowmeter.h"

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR flowSensorISR0() { flowMeterManager.handleInterrupt(0); }
void IRAM_ATTR flowSensorISR1() { flowMeterManager.handleInterrupt(1); }
void IRAM_ATTR flowSensorISR2() { flowMeterManager.handleInterrupt(2); }
void IRAM_ATTR flowSensorISR3() { flowMeterManager.handleInterrupt(3); }

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    flowMeterManager.timerUpdate();
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
    Serial.begin(9600);
    flowMeterManager.begin();
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
    flowMeterManager.printSerialValues();
    delay(1000); // Ausgabe jede Sekunde
}

