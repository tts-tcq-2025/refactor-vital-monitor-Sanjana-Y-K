#include "./monitor.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>  // for std::find_if

using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

bool isTemperatureCritical(float temperature) {
  return temperature > 102 || temperature < 95;
}

bool isPulseRateOutOfRange(float pulseRate) {
  return pulseRate < 60 || pulseRate > 100;
}

bool isSpo2Low(float spo2) {
  return spo2 < 90;
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

  // Use std::find_if instead of raw loop
  auto it = std::find_if(std::begin(checks), std::end(checks),
                         [](const Check& check) {
                           return check.func(check.value);
                         });

  if (it != std::end(checks)) {
    blinkWarningMessage(it->message);
    return 0;
  }
  return 1;
}
