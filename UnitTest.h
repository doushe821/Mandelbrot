#ifndef UNIT_TEST_H_INCLUDED
#define UNIT_TEST_H_INCLUDED

#include <stdio.h>

enum TestExitCode
{
    TEST_SUCCESS = 0,
    FP_DATA_NULL = -1,
    FP_INFO_NULL = -2,
    FP_PLOT_RAW_NULL = -3,
    FP_PLOT_OPTIMIZED_NULL = -4,
    ZERO_TEST_NUMBER = -5,
    ZERO_CPU_FREQUENCY = -6,
    SYSTEM_FUNCTION_CALL_FAILURE = -7,

};
enum TestExitCode UnitTest(const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius, const int TestNumber, const unsigned long long CPUfrequency, FILE* fpData, const char* DataFileName, FILE* fpInfo, FILE* fpPlotRaw, FILE* fpPlotOptimized);

#endif