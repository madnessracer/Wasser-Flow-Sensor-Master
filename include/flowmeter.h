
#// Copyright (c) 2025 madnessracer
#// MIT License
#
#pragma once
#include <Arduino.h>
#include <Adafruit_FRAM_I2C.h>

#define NUM_SENSORS 4
#define PULSES_PER_LITER 450.0
const uint8_t FLOW_SENSOR_PINS[NUM_SENSORS] = {2, 3, 4, 5};


class FlowMeterManager {
public:
    FlowMeterManager() : fram() {
        for (int i = 0; i < NUM_SENSORS; i++) {
            pulseCount[i] = 0;
            flowRate[i] = 0.0;
            totalLiters[i] = 0.0;
        }
    }
    void begin() {
        fram.begin(0x50);
        loadFromFRAM();
        for (int i = 0; i < NUM_SENSORS; i++) {
            pinMode(FLOW_SENSOR_PINS[i], INPUT_PULLUP);
        }
    }
    void handleInterrupt(uint8_t idx) {
        pulseCount[idx]++;
    }
    void timerUpdate() {
        for (int i = 0; i < NUM_SENSORS; i++) {
            flowRate[i] = (pulseCount[i] / PULSES_PER_LITER) * 60.0;
            totalLiters[i] += (pulseCount[i] / PULSES_PER_LITER);
            pulseCount[i] = 0;
        }
        saveToFRAM();
    }
    void loadFromFRAM() {
        for (int i = 0; i < NUM_SENSORS; i++) {
            uint32_t addr = i * sizeof(float);
            float val = 0.0;
            fram.read(addr, (uint8_t*)&val, sizeof(float));
            totalLiters[i] = val;
        }
    }
    void saveToFRAM() {
        for (int i = 0; i < NUM_SENSORS; i++) {
            uint32_t addr = i * sizeof(float);
            fram.write(addr, (uint8_t*)&totalLiters[i], sizeof(float));
        }
    }
    float getFlowRate(uint8_t idx) {
        return flowRate[idx];
    }
    float getTotalLiters(uint8_t idx) {
        return totalLiters[idx];
    }
    void reset() {
        for (int i = 0; i < NUM_SENSORS; i++) {
            totalLiters[i] = 0.0;
            saveToFRAM();
        }
    }
    void printSerialValues() {
        extern portMUX_TYPE timerMux;
        portENTER_CRITICAL(&timerMux);
        for (int i = 0; i < NUM_SENSORS; i++) {
            Serial.print("Sensor ");
            Serial.print(i+1);
            Serial.print(": Durchflussrate: ");
            Serial.print(getFlowRate(i), 2);
            Serial.print(" L/min, Gesamt: ");
            Serial.print(getTotalLiters(i), 2);
            Serial.println(" L");
        }
        Serial.println("----------------------");
        portEXIT_CRITICAL(&timerMux);
    }
private:
    volatile uint32_t pulseCount[NUM_SENSORS];
    volatile float flowRate[NUM_SENSORS];
    volatile float totalLiters[NUM_SENSORS];
    Adafruit_FRAM_I2C fram;
};

FlowMeterManager flowMeterManager;
