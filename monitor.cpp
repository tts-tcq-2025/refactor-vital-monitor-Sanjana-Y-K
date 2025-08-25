#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>

using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::milliseconds;

constexpr float TEMP_MIN = 95.0f;
constexpr float TEMP_MAX = 102.0f;
constexpr float PULSE_MIN = 60.0f;
constexpr float PULSE_MAX = 100.0f;
constexpr float SPO2_THRESHOLD = 90.0f;

constexpr int OK = 1;
constexpr int NOT_OK = 0;

constexpr float TEMP_TOLERANCE = 1.5f;  // Fixed 1.5 degrees instead of percentage

void blinkAlert() {
    for (int i = 0; i < 6; ++i) {
        cout << "\r**" << flush;
        sleep_for(milliseconds(800));
        cout << "\r  " << flush;
        sleep_for(milliseconds(800));
    }
    cout << "\r  \r" << flush;
}

void checkHypothermiaWarning(float temp) {
    if (temp >= TEMP_MIN && temp <= TEMP_MIN + TEMP_TOLERANCE) {
        cout << "Caution: Temperature approaching hypothermia range\n";
    }
}

void checkHyperthermiaWarning(float temp) {
    if (temp >= TEMP_MAX - TEMP_TOLERANCE && temp <= TEMP_MAX) {
        cout << "Caution: Temperature approaching hyperthermia range\n";
    }
}

int isTemperatureOk(float temp) {
    if (temp < TEMP_MIN || temp > TEMP_MAX) {
        cout << "Alert: Critical temperature detected!\n";
        blinkAlert();
        return NOT_OK;
    }
    checkHypothermiaWarning(temp);
    checkHyperthermiaWarning(temp);
    cout << "Temperature within normal range.\n";
    return OK;
}

int isPulseRateOk(float pulse) {
    if (pulse < PULSE_MIN || pulse > PULSE_MAX) {
        cout << "Alert: Abnormal pulse rate detected!\n";
        blinkAlert();
        return NOT_OK;
    }
    cout << "Pulse rate within normal range.\n";
    return OK;
}

int isSpO2Ok(float spo2) {
    if (spo2 < SPO2_THRESHOLD) {
        cout << "Alert: Oxygen saturation too low!\n";
        blinkAlert();
        return NOT_OK;
    }
    cout << "SpO2 within acceptable range.\n";
    return OK;
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    return isTemperatureOk(temperature) &&
           isPulseRateOk(pulseRate) &&
           isSpO2Ok(spo2);
}
