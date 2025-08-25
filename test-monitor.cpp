#include <cassert>
#include <iostream>
#include "./monitor.h"

using std::cout;

void test_isTemperatureOk() {
    cout << "\nTesting isTemperatureOk...\n";

    assert(isTemperatureOk(98.6f) == 1);   // Normal
    assert(isTemperatureOk(95.0f) == 1);   // Lower edge
    assert(isTemperatureOk(102.0f) == 1);  // Upper edge
    assert(isTemperatureOk(94.0f) == 0);   // Too low
    assert(isTemperatureOk(103.0f) == 0);  // Too high

    cout << "isTemperatureOk passed all tests.\n";
}

void test_isPulseRateOk() {
    cout << "\nTesting isPulseRateOk...\n";

    assert(isPulseRateOk(70.0f) == 1);     // Normal
    assert(isPulseRateOk(60.0f) == 1);     // Lower edge
    assert(isPulseRateOk(100.0f) == 1);    // Upper edge
    assert(isPulseRateOk(50.0f) == 0);     // Too low
    assert(isPulseRateOk(120.0f) == 0);    // Too high

    cout << "isPulseRateOk passed all tests.\n";
}

void test_isSpO2Ok() {
    cout << "\nTesting isSpO2Ok...\n";

    assert(isSpO2Ok(95.0f) == 1);     // Normal
    assert(isSpO2Ok(90.0f) == 1);     // Minimum acceptable
    assert(isSpO2Ok(85.0f) == 0);     // Too low

    cout << "isSpO2Ok passed all tests.\n";
}

void test_vitalsOk() {
    cout << "\nTesting vitalsOk...\n";

    assert(vitalsOk(98.6f, 75.0f, 95.0f) == 1);  // All normal
    assert(vitalsOk(94.0f, 75.0f, 95.0f) == 0);  // Temp low
    assert(vitalsOk(98.6f, 110.0f, 95.0f) == 0); // Pulse high
    assert(vitalsOk(98.6f, 75.0f, 85.0f) == 0);  // SpO2 low

    cout << "vitalsOk passed all tests.\n";
}

int main() {
    test_isTemperatureOk();
    test_isPulseRateOk();
    test_isSpO2Ok();
    test_vitalsOk();

    cout << "\nAll tests completed successfully.\n";
    return 0;
}

