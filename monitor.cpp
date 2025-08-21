#include "./monitor.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>  // for std::find_if
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

constexpr float TEMP_LOW = 95.0f;
constexpr float TEMP_HIGH = 102.0f;
constexpr float PULSE_LOW = 60.0f;
constexpr float PULSE_HIGH = 100.0f;
constexpr float SPO2_LOW = 90.0f;
constexpr float SPO2_HIGH = 100.0f;

// Calculate warning tolerance = 1.5% of upper limit
inline float warningTolerance(float upper_limit) {
  return upper_limit * 0.015f;
}

bool isTemperatureCritical(float temperature) {
  return temperature > TEMP_HIGH || temperature < TEMP_LOW;
}

bool isPulseRateOutOfRange(float pulseRate) {
  return pulseRate < PULSE_LOW || pulseRate > PULSE_HIGH;
}

bool isSpo2Low(float spo2) {
  return spo2 < SPO2_LOW;
}

// New warning checks:
// Returns warning message or empty string if no warning
std::string temperatureWarning(float temperature) {
  float tol = warningTolerance(TEMP_HIGH);
  if (temperature >= TEMP_LOW && temperature <= TEMP_LOW + tol) {
    return "Warning: Approaching hypothermia";
  }
  if (temperature >= TEMP_HIGH - tol && temperature <= TEMP_HIGH) {
    return "Warning: Approaching hyperthermia";
  }
  return "";
}

std::string pulseWarning(float pulseRate) {
  float tol = warningTolerance(PULSE_HIGH);
  if (pulseRate >= PULSE_LOW && pulseRate <= PULSE_LOW + tol) {
    return "Warning: Approaching low pulse rate";
  }
  if (pulseRate >= PULSE_HIGH - tol && pulseRate <= PULSE_HIGH) {
    return "Warning: Approaching high pulse rate";
  }
  return "";
}

std::string spo2Warning(float spo2) {
  float tol = warningTolerance(SPO2_HIGH);
  if (spo2 >= SPO2_LOW && spo2 <= SPO2_LOW + tol) {
    return "Warning: Approaching low SPO2 level";
  }
  return "";
}

void blinkWarningMessage(const char* message) {
  cout << message << '\n';
  for (int i = 0; i < 6; i++) {
    cout << "\r* " << flush;
    sleep_for(seconds(1));
    cout << "\r *" << flush;
    sleep_for(seconds(1));
  }
  cout << "\r  \r" << flush;  // Clear the line after blinking
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
  struct Check {
    bool (*func)(float);
    float value;
    const char* message;
  };
  
  const Check checks[] = {
    { isTemperatureCritical, temperature, "Temperature is critical!" },
    { isPulseRateOutOfRange, pulseRate, "Pulse Rate is out of range!" },
    { isSpo2Low, spo2, "Oxygen Saturation out of range!" }
  };

  // Check for critical vitals first
  auto it = std::find_if(std::begin(checks), std::end(checks),
                        [](const Check& check) {
                          return check.func(check.value);
                        });
  if (it != std::end(checks)) {
    blinkWarningMessage(it->message);
    return 0;
  }
  
  // Check and print warnings if applicable
  std::string warnTemp = temperatureWarning(temperature);
  if (!warnTemp.empty()) cout << warnTemp << std::endl;

  std::string warnPulse = pulseWarning(pulseRate);
  if (!warnPulse.empty()) cout << warnPulse << std::endl;

  std::string warnSpo2 = spo2Warning(spo2);
  if (!warnSpo2.empty()) cout << warnSpo2 << std::endl;

  return 1;
}
