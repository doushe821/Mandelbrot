#include "Benchmark.h"

#include "MandelbrotCalculation.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

const int MAX_DATA_STRING_LENGTH = 10;


enum ErrorCodes Benchmark(RunParameters params, FILE* fpData, FILE* fpInfo, FILE* fpPlotRaw, FILE* fpPlotOptimized)
{
    if(fpData == NULL)
    {
        return FP_DATA_NULL;
    }   
    if(fpInfo == NULL)
    {
        return FP_INFO_NULL;
    }
    if(fpPlotRaw == NULL)
    {
        return FP_PLOT_RAW_NULL;
    }
    if(fpPlotOptimized == NULL)
    {
        return FP_PLOT_OPTIMIZED_NULL;
    }
    if(params.TestNumber == 0)
    {
        return ZERO_TEST_NUMBER;
    }
    if(params.CPUFrequency == 0)
    {
        return ZERO_CPU_FREQUENCY;
    }
    double* LatencyDataArrayRaw = (double*)calloc((size_t)params.TestNumber, sizeof(double));
    if(LatencyDataArrayRaw == NULL)
    {
        return ALLOCATION_FAILURE;
    }
    double* LatencyDataArrayOptimized = (double*)calloc((size_t)params.TestNumber, sizeof(double));
    if(LatencyDataArrayOptimized == NULL)
    {
        return ALLOCATION_FAILURE;
    }
    double* LatencyDataArrayArrays = (double*)calloc((size_t)params.TestNumber, sizeof(double));
    if(LatencyDataArrayArrays == NULL)
    {
        return ALLOCATION_FAILURE;  
    }

    unsigned long long ClocksRaw = 0;
    unsigned long long ClocksOptimized = 0;
    unsigned long long ClocksArrays = 0;

    int32_t* PixelSet = (int*)calloc((size_t)(params.ScreenX * params.ScreenY), sizeof(int));

    unsigned long long start = 0;
    unsigned long long end = 0;

    for(int i = 0; i < params.TestNumber; i++)
    {
        start = _rdtsc(); 
        MandelbrotNaive(PixelSet, params.ScreenX, params.ScreenY, params.ProbeNumber, params.step, params.CenterX, params.CenterY, params.BorderRadius);
        end = _rdtsc();

        unsigned long long DeltaClocks = end - start;
        LatencyDataArrayRaw[i] = (double)(DeltaClocks) / (double)params.CPUFrequency;
        ClocksRaw += DeltaClocks;

        start = _rdtsc();
        MandelbrotIntrinsics(PixelSet, params.ScreenX, params.ScreenY, params.ProbeNumber, params.step, params.CenterX, params.CenterY, params.BorderRadius);
        end = _rdtsc();

        DeltaClocks = end - start;
        LatencyDataArrayOptimized[i] = (double)(DeltaClocks) / (double)params.CPUFrequency;
        ClocksOptimized += DeltaClocks; 


        start = _rdtsc();
        MandelbrotArrays(PixelSet, params.ScreenX, params.ScreenY, params.ProbeNumber, params.step, params.CenterX, params.CenterY, params.BorderRadius);
        end = _rdtsc();

        DeltaClocks = end - start;
        LatencyDataArrayArrays[i] = (double)(DeltaClocks) / (double)params.CPUFrequency;
        ClocksArrays += DeltaClocks; 
    }

    free(PixelSet);

    double RawLatency = (double)ClocksRaw / (double)params.CPUFrequency;
    double OptimizedLatency = (double)ClocksOptimized / (double)params.CPUFrequency; 
    double ArraysLatency = (double)ClocksArrays / (double)params.CPUFrequency; 

    fprintf(fpInfo, "%s\nCPU frequency = %llu\nNumber of tests = %d\nNumber of probes for each dot: %d\nShort summary:\nNaive latency = %lg; average naive latency = %lg\n"
    "Optimized latency = %lg; average optimized latency = %lg\nRelative Performance increase: %lg\nArrays latency: %lg\n" , 
    __TIME__, params.CPUFrequency, params.TestNumber, params.ProbeNumber, RawLatency, RawLatency / params.TestNumber,
    OptimizedLatency, OptimizedLatency / params.TestNumber, (double)ClocksRaw / (double)ClocksOptimized, ArraysLatency);

    char* DataBuffer = (char*)calloc((size_t)(MAX_DATA_STRING_LENGTH) * (size_t)params.TestNumber * 2, sizeof(char*));
    int lastPrinted = 0;
    for(int i = 0; i < params.TestNumber; i++)
    {
        lastPrinted += sprintf(DataBuffer + lastPrinted, "%.6lg\n",LatencyDataArrayRaw[i]);
    }
    for(int i = 0; i < params.TestNumber; i++)
    {
        lastPrinted += sprintf(DataBuffer + lastPrinted, "%.6lg\n", LatencyDataArrayOptimized[i]);
    }

    fwrite(DataBuffer, sizeof(char), strlen(DataBuffer), fpData);

    free(DataBuffer);

    fprintf(fpPlotRaw, "import matplotlib.pyplot as plt\n\nwith open('%s.dat', 'r') as f:\n"
    "   data = [float(line.strip()) for line in f]\n"
    "\n\nplt.hist(data[1:%d], bins = 30, color = 'blue', edgecolor = 'black', alpha = 0.7)\n"
    "plt.grid(axis='y', linestyle='--')\n"
    "plt.title('Latency distribution (raw)')\n"
    "plt.xlabel('Latency')\n"
    "plt.ylabel('Quantity')\n"
    "plt.savefig('%s.png', dpi = 300)", params.DataFname, params.TestNumber, params.OptimizedPlotFname);


    fprintf(fpPlotOptimized, "import matplotlib.pyplot as plt\n\nwith open('%s.dat', 'r') as f:\n"
    "   data = [float(line.strip()) for line in f]\n"
    "\n\nplt.hist(data[%d:%d], bins = 30, color = 'blue', edgecolor = 'black', alpha = 0.7)\n"
    "plt.grid(axis='y', linestyle='--')\n"
    "plt.title('Latency distribution (optimized)')\n"
    "plt.xlabel('Latency')\n"
    "plt.ylabel('Quantity')\n"
    "plt.savefig('%s.png', dpi = 300)", params.DataFname, params.TestNumber + 1, 2 * params.TestNumber, params.OptimizedPlotFname);
   

    free(LatencyDataArrayOptimized);
    free(LatencyDataArrayRaw);
    free(LatencyDataArrayArrays);

    return MODULE_SUCCESS;
}
