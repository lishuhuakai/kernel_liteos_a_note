#include "math_data_test.h"

uint64_t ConvertDoubleToBiased(const double& value)
{
    DoubleShape u;
    u.value = value;
    if (u.bits.sign) {
        return ~u.sign_magnitude + 1;
    } else {
        u.bits.sign = 1;
        return u.sign_magnitude;
    }
}

bool DoubleUlpCmp(double expected, double actual, uint64_t ulp)
{
    if (!isnan(expected) && !isnan(actual)) {
        uint64_t value_diff = ConvertDoubleToBiased(expected) >= ConvertDoubleToBiased(actual) ?
        (ConvertDoubleToBiased(expected) - ConvertDoubleToBiased(actual)) :
        (ConvertDoubleToBiased(actual) - ConvertDoubleToBiased(expected));
        return value_diff <= ulp;
    }
    return false;
}

uint32_t ConvertFloatToBiased(const float& value)
{
    FloatShape u;
    u.value = value;
    if (u.bits.sign) {
        return ~u.sign_magnitude + 1;
    } else {
        u.bits.sign = 1;
        return u.sign_magnitude;
    }
}
bool FloatUlpCmp(float expected, float actual, uint32_t ulp)
{
    if (!isnan(expected) && !isnan(actual)) {
        uint32_t value_diff = ConvertFloatToBiased(expected) >= ConvertFloatToBiased(actual) ?
        (ConvertFloatToBiased(expected) - ConvertFloatToBiased(actual)) :
        (ConvertFloatToBiased(actual) - ConvertFloatToBiased(expected));
        return value_diff <= ulp;
    }
    return false;
}