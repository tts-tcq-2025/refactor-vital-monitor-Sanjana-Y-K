#include "./monitor.h"
#include <iostream>
#include <thread>
#include <chrono>
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
  if (isTemperatureCritical(temperature)) {
    blinkWarningMessage("Temperature is critical!");
    return 0;
  }
  if (isPulseRateOutOfRange(pulseRate)) {
    blinkWarningMessage("Pulse Rate is out of range!");
    return 0;
  }
  if (isSpo2Low(spo2)) {
    blinkWarningMessage("Oxygen Saturation out of range!");
    return 0;
  }
  return 1;
}
