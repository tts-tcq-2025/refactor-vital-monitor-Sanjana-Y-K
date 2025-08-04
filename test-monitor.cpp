#include <gtest/gtest.h>
#include "./monitor.h"

TEST(MonitorValidation, TemperatureChecks) {
  EXPECT_TRUE(isTemperatureCritical(103));
  EXPECT_TRUE(isTemperatureCritical(94));
  EXPECT_FALSE(isTemperatureCritical(98.6));
}

TEST(MonitorValidation, PulseRateChecks) {
  EXPECT_TRUE(isPulseRateOutOfRange(59));
  EXPECT_TRUE(isPulseRateOutOfRange(101));
  EXPECT_FALSE(isPulseRateOutOfRange(75));
}

TEST(MonitorValidation, Spo2Checks) {
  EXPECT_TRUE(isSpo2Low(89));
  EXPECT_FALSE(isSpo2Low(95));
}

TEST(MonitorVitalsOk, ReturnsFalseIfAnyVitalIsOffRange) {
  EXPECT_FALSE(vitalsOk(103, 80, 98));   // Temp critical
  EXPECT_FALSE(vitalsOk(98, 50, 98));    // Pulse low
  EXPECT_FALSE(vitalsOk(98, 80, 85));    // Spo2 low
}

TEST(MonitorVitalsOk, ReturnsTrueIfAllVitalsNormal) {
  EXPECT_TRUE(vitalsOk(98.6, 70, 97));
}
