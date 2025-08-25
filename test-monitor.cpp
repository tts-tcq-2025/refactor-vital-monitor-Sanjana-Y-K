#include <gtest/gtest.h>
#include "./monitor.h"
#include <iostream>
#include <cassert>

int main() {
  
    assert(vitalsOk(98.6f, 80.0f, 95.0f) == true);
    assert(vitalsOk(95.5f, 80.0f, 95.0f) == true);
    assert(vitalsOk(93.0f, 80.0f, 95.0f) == false);
    assert(vitalsOk(101.5f, 80.0f, 95.0f) == true);
    assert(vitalsOk(103.0f, 80.0f, 95.0f) == false);
    assert(vitalsOk(98.6f, 61.0f, 95.0f) == true);
    assert(vitalsOk(98.6f, 55.0f, 95.0f) == false);
    assert(vitalsOk(98.6f, 101.0f, 95.0f) == true);
    assert(vitalsOk(98.6f, 110.0f, 95.0f) == false);
    assert(vitalsOk(98.6f, 80.0f, 91.0f) == true);
    assert(vitalsOk(98.6f, 80.0f, 85.0f) == false);
    assert(vitalsOk(93.0f, 80.0f, 85.0f) == false);

    std::cout << "All assert-based tests passed successfully.\n";
    return 0;
}


