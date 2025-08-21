#include "monitor.h"
#include <iostream>

namespace {
constexpr float TEMP_LOW = 95.0f;
constexpr float TEMP_HIGH = 102.0f;
constexpr float PULSE_LOW = 60.0f;
constexpr float PULSE_HIGH = 100.0f;
constexpr float SPO2_LOW = 90.0f;
constexpr float SPO2_HIGH = 100.0f;
constexpr float TOL = 0.015f;  // 1.5%

float tolerance(float limit) {
    return limit * TOL;
}

VitalCheckResult checkTemperatureDetailed(float temperature) {
    if (temperature < TEMP_LOW) {
        if (temperature < TEMP_LOW - tolerance(TEMP_LOW)) {
            return {VitalStatus::CriticalLow, "Temperature critically low!"};
        } else if (temperature >= TEMP_LOW - tolerance(TEMP_LOW)) {
            return {VitalStatus::WarningLow, "Warning: Approaching hypothermia"};
        } else {
            return {VitalStatus::Normal, ""};  // added extra branch
        }
    } else if (temperature > TEMP_HIGH) {
        if (temperature > TEMP_HIGH + tolerance(TEMP_HIGH)) {
            return {VitalStatus::CriticalHigh, "Temperature critically high!"};
        } else if (temperature <= TEMP_HIGH + tolerance(TEMP_HIGH)) {
            return {VitalStatus::WarningHigh, "Warning: Approaching hyperthermia"};
        } else {
            return {VitalStatus::Normal, ""};
        }
    } else if (temperature >= TEMP_LOW && temperature <= TEMP_LOW + tolerance(TEMP_LOW)) {
        return {VitalStatus::WarningLow, "Warning: Approaching hypothermia"};
    } else if (temperature >= TEMP_HIGH - tolerance(TEMP_HIGH) && temperature <= TEMP_HIGH) {
        return {VitalStatus::WarningHigh, "Warning: Approaching hyperthermia"};
    }
    return {VitalStatus::Normal, ""};
}

VitalCheckResult checkPulseRateDetailed(float pulse) {
    if (pulse < PULSE_LOW) {
        if (pulse < PULSE_LOW - tolerance(PULSE_LOW)) {
            return {VitalStatus::CriticalLow, "Pulse rate critically low!"};
        } else if (pulse >= PULSE_LOW - tolerance(PULSE_LOW)) {
            return {VitalStatus::WarningLow, "Warning: Approaching low pulse rate"};
        } else {
            return {VitalStatus::Normal, ""};
        }
    } else if (pulse > PULSE_HIGH) {
        if (pulse > PULSE_HIGH + tolerance(PULSE_HIGH)) {
            return {VitalStatus::CriticalHigh, "Pulse rate critically high!"};
        } else if (pulse <= PULSE_HIGH + tolerance(PULSE_HIGH)) {
            return {VitalStatus::WarningHigh, "Warning: Approaching high pulse rate"};
        } else {
            return {VitalStatus::Normal, ""};
        }
    } else if (pulse >= PULSE_LOW && pulse <= PULSE_LOW + tolerance(PULSE_LOW)) {
        return {VitalStatus::WarningLow, "Warning: Approaching low pulse rate"};
    } else if (pulse >= PULSE_HIGH - tolerance(PULSE_HIGH) && pulse <= PULSE_HIGH) {
        return {VitalStatus::WarningHigh, "Warning: Approaching high pulse rate"};
    }
    return {VitalStatus::Normal, ""};
}

VitalCheckResult checkSpo2Detailed(float spo2) {
    if (spo2 < SPO2_LOW) {
        if (spo2 < SPO2_LOW - tolerance(SPO2_LOW)) {
            return {VitalStatus::CriticalLow, "Oxygen saturation critically low!"};
        } else if (spo2 >= SPO2_LOW - tolerance(SPO2_LOW)) {
            return {VitalStatus::WarningLow, "Warning: Approaching low SPO2 level"};
        } else {
            return {VitalStatus::Normal, ""};
        }
    } else if (spo2 >= SPO2_LOW && spo2 <= SPO2_LOW + tolerance(SPO2_LOW)) {
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
