#include "monitor.h"
#include <iostream>

namespace {
constexpr float TEMP_LOW = 95.0f;
constexpr float TEMP_HIGH = 102.0f;
constexpr float PULSE_LOW = 60.0f;
constexpr float PULSE_HIGH = 100.0f;
constexpr float SPO2_LOW = 90.0f;
constexpr float TOL = 0.015f;  // 1.5%

float tolerance(float limit) {
    return limit * TOL;
}

// Helper to check a value against low/high thresholds with tolerance
VitalCheckResult checkVital(float value, float low, float high, 
                           const char* lowCriticalMsg, const char* lowWarnMsg, 
                           const char* highCriticalMsg, const char* highWarnMsg) {
    float lowTol = low - tolerance(low);
    float highTol = high + tolerance(high);

    if (value < lowTol) {
        return {VitalStatus::CriticalLow, lowCriticalMsg};
    }
    if (value < low) {
        return {VitalStatus::WarningLow, lowWarnMsg};
    }
    if (value > highTol) {
        return {VitalStatus::CriticalHigh, highCriticalMsg};
    }
    if (value > high) {
        return {VitalStatus::WarningHigh, highWarnMsg};
    }
    return {VitalStatus::Normal, ""};
}

VitalCheckResult checkTemperatureDetailed(float temperature) {
    return checkVital(temperature, TEMP_LOW, TEMP_HIGH,
                      "Temperature critically low!", "Warning: Approaching hypothermia",
                      "Temperature critically high!", "Warning: Approaching hyperthermia");
}

VitalCheckResult checkPulseRateDetailed(float pulse) {
    return checkVital(pulse, PULSE_LOW, PULSE_HIGH,
                      "Pulse rate critically low!", "Warning: Approaching low pulse rate",
                      "Pulse rate critically high!", "Warning: Approaching high pulse rate");
}

// SPO2 only has low thresholds
VitalCheckResult checkSpo2Detailed(float spo2) {
    float lowTol = SPO2_LOW - tolerance(SPO2_LOW);
    if (spo2 < lowTol) {
        return {VitalStatus::CriticalLow, "Oxygen saturation critically low!"};
    }
    if (spo2 < SPO2_LOW) {
        return {VitalStatus::WarningLow, "Warning: Approaching low SPO2 level"};
    }
    return {VitalStatus::Normal, ""};
}
}  // namespace

VitalCheckResult checkTemperature(float temperature) {
    return checkTemperatureDetailed(temperature);
}

VitalCheckResult checkPulseRate(float pulseRate) {
    return checkPulseRateDetailed(pulseRate);
}

VitalCheckResult checkSpo2(float spo2) {
    return checkSpo2Detailed(spo2);
}

void printMessage(const std::string& message) {
    if (!message.empty()) {
        std::cout << message << std::endl;
    }
}

bool vitalsOk(float temp, float pulse, float spo2) {
    VitalCheckResult results[] = {
        checkTemperature(temp),
        checkPulseRate(pulse),
        checkSpo2(spo2)
    };

    bool criticalFound = false;
    for (const auto& res : results) {
        printMessage(res.message);
        if (res.status == VitalStatus::CriticalLow || res.status == VitalStatus::CriticalHigh) {
            criticalFound = true;
        }
    }
    return !criticalFound;
}
