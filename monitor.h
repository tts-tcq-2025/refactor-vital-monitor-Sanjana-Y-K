#ifndef MONITOR_H
#define MONITOR_H

#include <string>

enum class VitalStatus {
    CriticalLow,
    WarningLow,
    Normal,
    WarningHigh,
    CriticalHigh
};

struct VitalCheckResult {
    VitalStatus status;
    std::string message;
};

VitalCheckResult checkTemperature(float temperature);

VitalCheckResult checkPulseRate(float pulseRate);

VitalCheckResult checkSpo2(float spo2);

void printMessage(const std::string& message);

bool vitalsOk(float temp, float pulse, float spo2);

#endif // MONITOR_H
