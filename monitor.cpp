#include "monitor.h"
#include <iostream>
#include <string>

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

enum class VitalLimitType {
    TwoSided,  // low and high limits
    LowOnly    // only low limit
};

// Generalized vital check with tolerance and messages
VitalCheckResult checkVital(float value, float low, float high,
                           VitalLimitType limitType,
                           const std::string& lowCriticalMsg,
                           const std::string& lowWarnMsg,
                           const std::string& highCriticalMsg = "",
                           const std::string& highWarnMsg = "") {
    float lowTol = low - tolerance(low);

    if (limitType == VitalLimitType::TwoSided) {
        float highTol = high + tolerance(high);
        if (value < lowTol) {
            return VitalCheckResult{VitalStatus::CriticalLow, lowCriticalMsg};
        }
        if (value < low) {
            return VitalCheckResult{VitalStatus::WarningLow, lowWarnMsg};
        }
        if (value > highTol) {
            return VitalCheckResult{VitalStatus::CriticalHigh, highCriticalMsg};
        }
        if (value > high) {
            return VitalCheckResult{VitalStatus::WarningHigh, highWarnMsg};
        }
        return VitalCheckResult{VitalStatus::Normal, ""};
    }
    else {  // LowOnly limits
        if (value < lowTol) {
            return VitalCheckResult{VitalStatus::CriticalLow, lowCriticalMsg};
        }
        if (value < low) {
            return VitalCheckResult{VitalStatus::WarningLow, lowWarnMsg};
        }
        return VitalCheckResult{VitalStatus::Normal, ""};
    }
}

VitalCheckResult checkTemperatureDetailed(float temperature) {
    return checkVital(temperature, TEMP_LOW, TEMP_HIGH, VitalLimitType::TwoSided,
                      "Temperature critically low!", "Warning: Approaching hypothermia",
                      "Temperature critically high!", "Warning: Approaching hyperthermia");
}

VitalCheckResult checkPulseRateDetailed(float pulse) {
    return checkVital(pulse, PULSE_LOW, PULSE_HIGH, VitalLimitType::TwoSided,
                      "Pulse rate critically low!", "Warning: Approaching low pulse rate",
                      "Pulse rate critically high!", "Warning: Approaching high pulse rate");
}

VitalCheckResult checkSpo2Detailed(float spo2) {
    return checkVital(spo2, SPO2_LOW, 0.0f, VitalLimitType::LowOnly,
                      "Oxygen saturation critically low!", "Warning: Approaching low SPO2 level");
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
