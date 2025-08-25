#ifndef MONITOR_H
#define MONITOR_H

int isTemperatureOk(float temperature);
int isPulseRateOk(float pulseRate);
int isSpO2Ok(float spo2);
int vitalsOk(float temperature, float pulseRate, float spo2);

#endif // MONITOR_H
