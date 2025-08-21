#include "monitor.h"
#include <iostream>
#include <cmath>

namespace {
  constexpr float TEMP_LOW = 95.0f;
  constexpr float TEMP_HIGH = 102.0f;
  constexpr float PULSE_LOW = 60.0f;
  constexpr float PULSE_HIGH = 100.0f;
  constexpr float SPO2_LOW = 90.0f;
  constexpr float SPO2_HIGH = 100.0f;

  constexpr float TOLERANCE_PERCENT = 0.015f;  // 1.5%

  inline float tolerance(float upperLimit) {
    return upperLimit * TOLERANCE_PERCENT;
  }

  VitalCheckResult evaluateVital(float value, float lowLimit, float highLimit,
                                const std::string& name,
                                const std::string& lowWarning,
                                const std::string& highWarning,
                                const std::string& lowCritical,
                                const std::string& highCritical) {
    if (value < lowLimit) {
      if (value >= lowLimit - tolerance(lowLimit)) {
        return {VitalStatus::WarningLow, "Warning: Approaching " + lowWarning};
      }
      return {VitalStatus::CriticalLow, lowCritical};
    }
    if (value > highLimit) {
      if (value <= highLimit + tolerance(highLimit)) {
        return {VitalStatus::WarningHigh, "Warning: Approaching " + highWarning};
      }
      return {VitalStatus::CriticalHigh, highCritical};
    }
    // Check for warnings near lower limit
    if (value >= lowLimit && value <= lowLimit + tolerance(lowLimit)) {
      return {VitalStatus::WarningLow, "Warning: Approaching " + lowWarning};
    }
    // Check for warnings near upper limit
    if (value >= highLimit - tolerance(highLimit) && value <= highLimit) {
      return {VitalStatus::WarningHigh, "Warning: Approaching " + highWarning};
    }
    return {VitalStatus::Normal, ""};
  }
}

VitalCheckResult checkTemperature(float temperature) {
  return evaluateVital(temperature, TEMP_LOW, TEMP_HIGH,
                       "Temperature",
                       "hypothermia", "hyperthermia",
                       "Temperature is critically low!",
                       "Temperature is critically high!");
}

VitalCheckResult checkPulseRate(float pulseRate) {
  return evaluateVital(pulseRate, PULSE_LOW, PULSE_HIGH,
                       "Pulse rate",
                       "low pulse rate", "high pulse rate",
                       "Pulse rate is critically low!",
                       "Pulse rate is critically high!");
}

VitalCheckResult checkSpo2(float spo2) {
  return evaluateVital(spo2, SPO2_LOW, SPO2_HIGH,
                       "Oxygen saturation",
                       "low SPO2 level", "high SPO2 level",
                       "Oxygen saturation critically low!",
                       "Oxygen saturation critically high!");
}

void printMessage(const std::string& message) {
  if (!message.empty()) {
    std::cout << message << std::endl;
  }
}

bool vitalsOk(float temperature, float pulseRate, float spo2) {
  VitalCheckResult results[] = {
    checkTemperature(temperature),
    checkPulseRate(pulseRate),
    checkSpo2(spo2)
  };

  bool criticalFound = false;
  for (const auto& result : results) {
    printMessage(result.message);
    if (result.status == VitalStatus::CriticalLow ||
        result.status == VitalStatus::CriticalHigh) {
      criticalFound = true;
    }
  }
  return !criticalFound;
}
