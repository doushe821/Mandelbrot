#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

#include <SDL2/SDL.h>

#include "MandelbrotCalculation.h"
#include "UnitTestEdited.h"
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

    FILE* MainData = fopen("GTestData.csv", "w+b");

    const int TestsNumber = 20;
    params.TestNumber = 200;
    for(int i = 0; i < 20; i++)
    {
        params.ProbeNumber = 50 + i * 50;
        ErrorParser(FileNamesInit(&params));

        if(system("mkdir Plots") != 0)
        {
            ErrorParser(SYSTEM_FUNCTION_CALL_FAILURE);
        }

        FILE* fpData = fopen(params.DataFnameFullPath, "w+b");
        FILE* fpInfo = fopen(params.InfoFnameFullPath, "w+b");
        FILE* fpPlotRaw = fopen(params.RawPyFname, "w+b");
        FILE* fpPlotOpt = fopen(params.OptimizedPyFname, "w+b");
        
        ErrorParser(UnitTestEdited(params, fpData, fpInfo, fpPlotRaw, fpPlotOpt, MainData));

        fclose(fpData);
        fclose(fpInfo);
        fclose(fpPlotRaw);  
        fclose(fpPlotOpt);
    }

    return 0;
}