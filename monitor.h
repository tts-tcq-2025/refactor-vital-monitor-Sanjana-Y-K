#pragma once

#include <string>

enum class VitalStatus { Normal, Warning, Critical };

struct VitalCheckResult {
    VitalStatus status;
    std::string message;
};

VitalCheckResult checkTemperature(float temperature);
VitalCheckResult checkPulseRate(float pulseRate);
VitalCheckResult checkSpo2(float spo2);

bool vitalsOk(float temperature, float pulseRate, float spo2);
