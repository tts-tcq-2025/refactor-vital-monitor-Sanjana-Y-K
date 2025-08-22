#include "monitor.h"
#include <iostream>
#include <string>  // include string for std::string usage

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
        VitalCheckResult res;
        res.status = VitalStatus::CriticalLow;
        res.message = std::string(lowCriticalMsg);
        return res;
    }
    if (value < low) {
        VitalCheckResult res;
        res.status = VitalStatus::WarningLow;
        res.message = std::string(lowWarnMsg);
        return res;
    }
    if (value > highTol) {
        VitalCheckResult res;
        res.status = VitalStatus::CriticalHigh;
        res.message = std::string(highCriticalMsg);
        return res;
    }
    if (value > high) {
        VitalCheckResult res;
        res.status = VitalStatus::WarningHigh;
        res.message = std::string(highWarnMsg);
        return res;
    }
    VitalCheckResult res;
    res.status = VitalStatus::Normal;
    res.message = "";
    return res;
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

VitalCheckResult checkSpo2Detailed(float spo2) {
    float lowTol = SPO2_LOW - tolerance(SPO2_LOW);
    if (spo2 < lowTol) {
        VitalCheckResult res;
        res.status = VitalStatus::CriticalLow;
        res.message = "Oxygen saturation critically low!";
        return res;
    }
    if (spo2 < SPO2_LOW) {
        VitalCheckResult res;
        res.status = VitalStatus::WarningLow;
        res.message = "Warning: Approaching low SPO2 level";
        return res;
    }
    VitalCheckResult res;
    res.status = VitalStatus::Normal;
    res.message = "";
    return res;
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
