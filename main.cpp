#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

#include <SDL2/SDL.h>

#include "MandelbrotCalculation.h"
#include "Benchmark.h"
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
        ErrorParser(FileNamesInit(&params));

        if(system("mkdir -p Plots") != 0)
        {
            ErrorParser(SYSTEM_FUNCTION_CALL_FAILURE);
        }

        FILE* fpData = NULL;
        FILE* fpInfo = NULL;
        FILE* fpPlotNaive = NULL;
        FILE* fpPlotOpt = NULL;
        FILE* fpPlotArrays = NULL;

        fpData = fopen(params.DataFnameFullPath, "w+b"); 
        if(fpData == NULL)
        {
            fprintf(stderr, "Failed to open data file\n");
            goto DataFileFail;
        }
        fpInfo = fopen(params.InfoFnameFullPath, "w+b");
        if(fpInfo == NULL)
        {
            fprintf(stderr, "Failed to open info file\n");
            goto InfoFileFail;       
        }
        fpPlotNaive = fopen(params.NaivePyFname, "w+b");
        if(fpPlotNaive == NULL)
        {
            fprintf(stderr, "Failed to open naive plot file\n");
            goto NaivePlotFail;       
        }
        fpPlotOpt = fopen(params.OptimizedPyFname, "w+b");
        if(fpPlotOpt == NULL)
        {
            fprintf(stderr, "Failed to open instrinsics plot file\n");
            goto OptPlotFail;       
        }
        fpPlotArrays = fopen(params.ArraysPyFname, "w+b");
        if(fpPlotArrays == NULL)
        {
            fprintf(stderr, "Failed to open instrinsics plot file\n");
            goto ArraysPlotFail;       
        }
        
        ErrorParser(Benchmark(params, fpData, fpInfo, fpPlotNaive, fpPlotOpt, fpPlotArrays)); 

        fclose(fpPlotArrays);
        ArraysPlotFail: fclose(fpPlotOpt);
        OptPlotFail: fclose(fpPlotNaive);  
        NaivePlotFail: fclose(fpInfo);
        InfoFileFail: fclose(fpData);
    }

   //DataFileFail: if(params.GraphicsFlag)
   //{
   //    ErrorParser(DisplayPixelsSDL(params.ScreenX, params.ScreenY, params.ProbeNumber));
   //}

    DataFileFail: return 0;
}