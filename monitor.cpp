#include "monitor.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

namespace {
constexpr float TEMP_LOW = 95.0f;
constexpr float TEMP_HIGH = 102.0f;
constexpr float PULSE_LOW = 60.0f;
constexpr float PULSE_HIGH = 100.0f;
constexpr float SPO2_LOW = 90.0f;
constexpr float TOL = 0.015f;  // 1.5%

enum class VitalStatus { Normal, Warning, Critical };

// Simple struct for results
struct VitalCheckResult {
    VitalStatus status;
    std::string message;
};

// Blink alert for critical conditions
void blinkAlert() {
    for (int i = 0; i < 6; ++i) {
        std::cout << "\r* " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\r *" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "\r  \r" << std::flush;
}

// Check a vital with low/high limits (high can be 0 for low-only)
VitalCheckResult checkVital(float val, float low, float high,
                           const std::string& critLowMsg,
                           const std::string& warnLowMsg,
                           const std::string& critHighMsg = "",
                           const std::string& warnHighMsg = "") {
    float lowTol = low - low * TOL;
    if (val < lowTol) return {VitalStatus::Critical, critLowMsg};
    if (val < low) return {VitalStatus::Warning, warnLowMsg};
    if (high > 0) {
        float highTol = high + high * TOL;
        if (val > highTol) return {VitalStatus::Critical, critHighMsg};
        if (val > high) return {VitalStatus::Warning, warnHighMsg};
    }
    return {VitalStatus::Normal, ""};
}

}  // namespace

// Specific vital checks
VitalCheckResult checkTemperature(float t) {
    return checkVital(t, TEMP_LOW, TEMP_HIGH,
                      "Temperature critically low!", "Warning: Approaching hypothermia",
                      "Temperature critically high!", "Warning: Approaching hyperthermia");
}
VitalCheckResult checkPulseRate(float p) {
    return checkVital(p, PULSE_LOW, PULSE_HIGH,
                      "Pulse rate critically low!", "Warning: Approaching low pulse rate",
                      "Pulse rate critically high!", "Warning: Approaching high pulse rate");
}
VitalCheckResult checkSpo2(float s) {
    return checkVital(s, SPO2_LOW, 0.0f,
                      "Oxygen saturation critically low!", "Warning: Approaching low SPO2 level");
}

// Print message if non-empty
void printMessage(const std::string& msg) {
    if (!msg.empty()) std::cout << msg << std::endl;
}

// Check all vitals, blink on critical, print warnings
bool vitalsOk(float temp, float pulse, float spo2) {
    auto results = {checkTemperature(temp), checkPulseRate(pulse), checkSpo2(spo2)};
    bool critical = false;

    for (const auto& r : results) {
        printMessage(r.message);
        if (r.status == VitalStatus::Critical) critical = true;
    }
    if (critical) blinkAlert();
    return !critical;
}
