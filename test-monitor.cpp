#include <gtest/gtest.h>
#include "monitor.h"

TEST(MonitorCheck, Temperature) {
  EXPECT_EQ(checkTemperature(103).status, VitalStatus::CriticalHigh);
  EXPECT_EQ(checkTemperature(94).status, VitalStatus::CriticalLow);
  EXPECT_EQ(checkTemperature(95.5).status, VitalStatus::WarningLow);
  EXPECT_EQ(checkTemperature(101.5).status, VitalStatus::WarningHigh);
  EXPECT_EQ(checkTemperature(98.6).status, VitalStatus::Normal);
}

TEST(MonitorCheck, PulseRate) {
  EXPECT_EQ(checkPulseRate(59).status, VitalStatus::CriticalLow);
  EXPECT_EQ(checkPulseRate(101).status, VitalStatus::CriticalHigh);
  EXPECT_EQ(checkPulseRate(61).status, VitalStatus::WarningLow);
  EXPECT_EQ(checkPulseRate(99).status, VitalStatus::WarningHigh);
  EXPECT_EQ(checkPulseRate(75).status, VitalStatus::Normal);
}

TEST(MonitorCheck, Spo2) {
  EXPECT_EQ(checkSpo2(89).status, VitalStatus::CriticalLow);
  EXPECT_EQ(checkSpo2(90.5).status, VitalStatus::WarningLow);
  EXPECT_EQ(checkSpo2(95).status, VitalStatus::Normal);
}

TEST(MonitorVitalsOk, ReturnsFalseWhenCritical) {
  EXPECT_FALSE(vitalsOk(103, 80, 98));  // Temp critical high
  EXPECT_FALSE(vitalsOk(98, 50, 98));   // Pulse critical low
  EXPECT_FALSE(vitalsOk(98, 80, 85));   // SPO2 critical low
}

TEST(MonitorVitalsOk, ReturnsTrueWhenNormalOrWarning) {
  EXPECT_TRUE(vitalsOk(95.5, 80, 98));   // Temp warning low
  EXPECT_TRUE(vitalsOk(102, 85, 90.5));  // Temp warning high, SPO2 warning low
  EXPECT_TRUE(vitalsOk(98.6, 75, 97));   // normal
}
