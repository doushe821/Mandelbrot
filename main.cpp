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

int main(int argc, char** argv)
{
    flags_t CMDFlags = {};
    RunParameters params = {};

    ErrorParser(ParseCMD(argc, argv, &CMDFlags));
    ErrorParser(InitParams(CMDFlags, &params));

    #ifndef NDEBUG
    printf("Using run parameters:\n");
    printf("Border radius = %f\nCenterX = %d\nCenterY = %d\nCPU frequency = %llu\nGraphics = %d\nProbe number = %d\nScreenX = %d\nScreenY = %d\nStep = %f\nTest number = %d\n", params.BorderRadius, params.CenterX, params.CenterY, params.CPUFrequency, params.GraphicsFlag, params.ProbeNumber, params.ScreenX, params.ScreenY, params.step, params.TestNumber);
    #endif

    if(params.TestNumber > 0)
    {
        FILE* fpData = fopen("data.dat", "w+b");
        FILE* fpInfo = fopen("info.txt", "w+b");
        FILE* fpPlotRaw = fopen("plotRaw.py", "w+b");
        FILE* fpPlotOpt = fopen("plotOptimized.py", "w+b");
        ErrorParser(UnitTest(params, fpData, "data.dat", fpInfo, fpPlotRaw, fpPlotOpt));

        fclose(fpData);
        fclose(fpInfo);
        fclose(fpPlotRaw);  
        fclose(fpPlotOpt);
    }
    if(params.GraphicsFlag)
    {
        ErrorParser(DisplayPixelsSDL(params.ScreenX, params.ScreenY, params.ProbeNumber));
    }

    return 0;
}