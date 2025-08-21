#include <gtest/gtest.h>
#include "monitor.h"

TEST(MonitorCritical, TemperatureCritical) {
  EXPECT_TRUE(isTemperatureCritical(103));
  EXPECT_TRUE(isTemperatureCritical(94));
  EXPECT_FALSE(isTemperatureCritical(98.6));
}

TEST(MonitorCritical, PulseRateOutOfRange) {
  EXPECT_TRUE(isPulseRateOutOfRange(59));
  EXPECT_TRUE(isPulseRateOutOfRange(101));
  EXPECT_FALSE(isPulseRateOutOfRange(75));
}

TEST(MonitorCritical, Spo2Low) {
  EXPECT_TRUE(isSpo2Low(89));
  EXPECT_FALSE(isSpo2Low(95));
}

TEST(MonitorWarnings, TemperatureWarning) {
  float tolerance = 102.0f * 0.015f;  // 1.5% of 102

  EXPECT_EQ(temperatureWarning(95.0f), "Warning: Approaching hypothermia");
  EXPECT_EQ(temperatureWarning(95.0f + tolerance / 2), "Warning: Approaching hypothermia");
  EXPECT_EQ(temperatureWarning(102.0f - tolerance / 2), "Warning: Approaching hyperthermia");
  EXPECT_EQ(temperatureWarning(102.0f), "Warning: Approaching hyperthermia");

  // Outside warning range returns empty string
  EXPECT_EQ(temperatureWarning(96.6f), "");
  EXPECT_EQ(temperatureWarning(101.0f), "");
}

TEST(MonitorWarnings, PulseWarning) {
  float tolerance = 100.0f * 0.015f;  // 1.5% of 100

  EXPECT_EQ(pulseWarning(60.0f), "Warning: Approaching low pulse rate");
  EXPECT_EQ(pulseWarning(60.0f + tolerance / 2), "Warning: Approaching low pulse rate");
  EXPECT_EQ(pulseWarning(100.0f - tolerance / 2), "Warning: Approaching high pulse rate");
  EXPECT_EQ(pulseWarning(100.0f), "Warning: Approaching high pulse rate");

  EXPECT_EQ(pulseWarning(65.0f), "");
  EXPECT_EQ(pulseWarning(95.0f), "");
}

TEST(MonitorWarnings, Spo2Warning) {
  float tolerance = 100.0f * 0.015f;  // 1.5% of 100

  EXPECT_EQ(spo2Warning(90.0f), "Warning: Approaching low SPO2 level");
  EXPECT_EQ(spo2Warning(90.0f + tolerance / 2), "Warning: Approaching low SPO2 level");

  EXPECT_EQ(spo2Warning(92.0f), "");
  EXPECT_EQ(spo2Warning(95.0f), "");
}

TEST(MonitorVitalsOk, ReturnsFalseOnCritical) {
  EXPECT_EQ(vitalsOk(103, 80, 98), 0);   // Temp critical
  EXPECT_EQ(vitalsOk(98, 50, 98), 0);    // Pulse low
  EXPECT_EQ(vitalsOk(98, 80, 85), 0);    // Spo2 low
}

TEST(MonitorVitalsOk, ReturnsTrueOnNormalWithWarnings) {
  // Values within warning range but not critical should return 1
  EXPECT_EQ(vitalsOk(95.5, 61.0, 91), 1);
}

TEST(MonitorVitalsOk, ReturnsTrueIfAllVitalsNormal) {
  EXPECT_EQ(vitalsOk(98.6, 70, 97), 1);
}
