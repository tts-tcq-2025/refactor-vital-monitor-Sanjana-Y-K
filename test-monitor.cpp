#include <gtest/gtest.h>
#include "./monitor.h"

TEST(Monitor, NotOkWhenAnyVitalIsOffRange) {
  ASSERT_FALSE(vitalsOk(99, 102, 70));
  ASSERT_TRUE(vitalsOk(98.1, 70, 98));
  ASSERT_TRUE(vitalsOk(95, 60, 90));
  ASSERT_TRUE(vitalsOk(102, 100, 90));
  ASSERT_FALSE(vitalsOk(94, 59, 89));
  ASSERT_FALSE(vitalsOk(103, 101, 91));
  ASSERT_TRUE(vitalsOk(101, 100, 90));
  ASSERT_TRUE(vitalsOk(96, 100, 90));
}
