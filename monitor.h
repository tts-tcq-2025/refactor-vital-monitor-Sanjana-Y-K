#ifndef MONITOR_H_
#define MONITOR_H_

#include <string>

enum class VitalStatus {
  Normal,
  WarningLow,
  WarningHigh,
  CriticalLow,
  CriticalHigh
};

struct VitalCheckResult {
  VitalStatus status;
  std::string message;
};

// Vital checks
VitalCheckResult checkTemperature(float temperature);
VitalCheckResult checkPulseRate(float pulseRate);
VitalCheckResult checkSpo2(float spo2);

// Shows warning or critical messages (non-blocking, no blinking)
void printMessage(const std::string& message);

// Main vitals check function
// Returns true if all vitals normal or warning, false if any is critical
bool vitalsOk(float temperature, float pulseRate, float spo2);

#endif  // MONITOR_H_
