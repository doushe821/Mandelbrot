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

int main(const int argc, const char** argv)
{
    if(argc > 1)
    {
        return *(int*)argv;
    }
    const int ScreenX = 800;
    const int ScreenY = 800;
    const int N = 200;
    const float step = 0.004f;
    const float BorderCircumferenceRadiusSquare = 3;
    const int CenterX = ScreenX/2;
    const int CenterY = ScreenY/2;
    const unsigned long long CPU_FREQUENCY = 2000000000;

    const int TestNumber = 1;

    FILE* fpData = fopen("data.dat", "w+b");
    FILE* fpInfo = fopen("info.txt", "w+b");
    FILE* fpPlotRaw = fopen("plotRaw.py", "w+b");
    FILE* fpPlotOpt = fopen("plotOptimized.py", "w+b");
    UnitTest(ScreenX, ScreenY, N, step, CenterX, CenterY, BorderCircumferenceRadiusSquare, TestNumber, CPU_FREQUENCY, fpData, "data.dat", fpInfo, fpPlotRaw, fpPlotOpt);

    fclose(fpData);
    fclose(fpInfo);
    fclose(fpPlotRaw);  
    fclose(fpPlotOpt);

    DisplayPixelsSDL(ScreenX, ScreenY, N);
    return 0;
}
