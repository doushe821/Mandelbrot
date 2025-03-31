#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

#include <SDL2/SDL.h>

#include "MandelbrotCalculation.h"
#include "UnitTest.h"
#include "Visualizer.h"
#include "ErrorParser.h"
#include "CMDParser.h"
#include "RunParametersStruct.h"

int main(const int argc, const char** argv)
{
    flags_t CMDFlags = {};
    RunParameters params = {};

    ErrorParser(ParseCMD(argc, argv, &CMDFlags));
    ErrorParser(InitParams(CMDFlags, &params));

    FILE* fpData = fopen("data.dat", "w+b");
    FILE* fpInfo = fopen("info.txt", "w+b");
    FILE* fpPlotRaw = fopen("plotRaw.py", "w+b");
    FILE* fpPlotOpt = fopen("plotOptimized.py", "w+b");
    UnitTest(params, fpData, "data.dat", fpInfo, fpPlotRaw, fpPlotOpt);

    fclose(fpData);
    fclose(fpInfo);
    fclose(fpPlotRaw);  
    fclose(fpPlotOpt);

    DisplayPixelsSDL(params.ScreenX, params.ScreenY, params.ProbeNumber);
    return 0;
}

enum ErrorCodes InitParams(flags_t CMDFlags, RunParameters* params)
{
    const int ScreenXdefault = 800;
    const int ScreenYdefault = 800;
    const float stepDefault = 0.003;
    const float BorderRadiusDefault = 3;
    const int TestNumberDefault = 500;
    const int ProbeNumberDefault = 255;
    const unsigned long long CPUFrequencyDefault = 3000000000;

    if(CMDFlags.isDimensions)
    {
        if(CMDFlags.ScreenX <= 0 || CMDFlags.ScreenY <= 0)
        {
            return INCORRECT_DIMENSION_VALUE;
        }
        params->ScreenX = CMDFlags.ScreenX;
        params->ScreenY = CMDFlags.ScreenY;
    }
    else
    {
        params->ScreenX =ScreenXdefault;
        params->ScreenY = ScreenYdefault;    
    }

    if(CMDFlags.isProbeNumber)
    {
        params->ProbeNumber = CMDFlags.ProbeNumber;
    }
    else
    {
        params->ProbeNumber = ProbeNumberDefault;
    }

    params->GraphicsFlag = CMDFlags.isGraphic;

    if(CMDFlags.isTest)
    {
        if(CMDFlags.TestNumber <= 0)
        {
            return INCORRECT_TEST_NUMBER_VALUE;
        }
        params->TestNumber = CMDFlags.TestNumber;
    }
    else
    {
        params->TestNumber = TestNumberDefault;
    }

    if(CMDFlags.isStep)
    {
        params->step = CMDFlags.step;
    }

    if(CMDFlags.CPUFrequency != 0)
    {
        params->CPUFrequency = CMDFlags.CPUFrequency;
    }
    else
    {
        params->CPUFrequency = CPUFrequencyDefault;
    }

    params->BorderRadius = BorderRadiusDefault;
    return MODULE_SUCCESS;
}