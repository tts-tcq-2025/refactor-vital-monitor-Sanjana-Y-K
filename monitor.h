#pragma once

bool isTemperatureCritical(float temperature);
bool isPulseRateOutOfRange(float pulseRate);
bool isSpo2Low(float spo2);

int vitalsOk(float temperature, float pulseRate, float spo2);


