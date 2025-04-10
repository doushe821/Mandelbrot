#ifndef UNIT_TEST_H_INCLUDED
#define UNIT_TEST_H_INCLUDED

#include <stdio.h>

#include "RunParametersStruct.h"
#include "ErrorParser.h"

enum ErrorCodes UnitTestEdited(RunParameters params, FILE* fpData, FILE* fpInfo, FILE* fpPlotRaw, FILE* fpPlotOptimized, FILE* MainData);

#endif