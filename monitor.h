#ifndef MONITOR_H_
#define MONITOR_H_

#include <string>

bool isTemperatureCritical(float temperature);
bool isPulseRateOutOfRange(float pulseRate);
bool isSpo2Low(float spo2);

std::string temperatureWarning(float temperature);
std::string pulseWarning(float pulseRate);
std::string spo2Warning(float spo2);

// Alarm blinking for critical warnings
void blinkWarningMessage(const char* message);

// Returns 1 if all vitals OK, 0 if critical alarm triggered
int vitalsOk(float temperature, float pulseRate, float spo2);

#endif  
