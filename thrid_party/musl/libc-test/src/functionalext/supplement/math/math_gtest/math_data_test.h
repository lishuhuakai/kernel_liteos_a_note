#ifndef MATH_GTEST_MATH_DATA_TEST_H
#define MATH_GTEST_MATH_DATA_TEST_H

#include <gtest/gtest.h>
#include <math.h>
#include <fenv.h>

struct DataDoubleDouble {
    double input;
    double expected;
};

struct DataFloatFloat {
    float input;
    float expected;
};

struct DataIntFloat {
    float input;
    int expected;
};

struct DataIntDouble {
    double input;
    int expected;
};

struct DataLongDouble {
    double input;
    long expected;
};

struct DataLongFloat {
    float input;
    long expected;
};

struct DataLlongDouble {
    double input;
    long long expected;
};

struct DataLlongFloat {
    float input;
    long long expected;
};

struct DataDoubleDoubleInt {
    double input1;
    int input2;
    double expected;
};

struct DataFloatFloatInt {
    float input1;
    int input2;
    float expected;
};

struct DataDouble3Expected1 {
    double input1;
    double input2;
    double expected;
};

struct DataFloat3Expected1 {
    float input1;
    float input2;
    float expected;
};

struct DataDouble3Expected2 {
    double input;
    double expected1;
    double expected2;
};

struct DataFloat3Expected2 {
    float input;
    float expected1;
    float expected2;
};

struct DataFloatIntFloat {
    float input;
    float expected1;
    int expected2;
};

struct DataDoubleIntDouble {
    double input;
    double expected1;
    int expected2;
};

struct DataDouble3Int1 {
    double input1;
    double input2;
    double expected1;
    int expected2;
};

struct DataFloat3Int1 {
    float input1;
    float input2;
    float expected1;
    int expected2;
};

struct DataDouble4 {
    double input1;
    double input2;
    double input3;
    double expected;
};

struct DataFloat4 {
    float input1;
    float input2;
    float input3;
    float expected;
};

union FloatShape {
    float value;
    struct {
        unsigned frac : 23;
        unsigned exp : 8;
        unsigned sign : 1;
    } bits;
    uint32_t sign_magnitude;
};

union DoubleShape {
    double value;
    struct {
        unsigned fracl;
        unsigned frach : 20;
        unsigned exp : 11;
        unsigned sign : 1;
    } bits;
    uint64_t sign_magnitude;
};

uint64_t ConvertDoubleToBiased(const double &value);
bool DoubleUlpCmp(double expected, double actual, uint64_t ulp);
uint32_t ConvertFloatToBiased(const float &value);
bool FloatUlpCmp(float expected, float actual, uint32_t ulp);

#endif