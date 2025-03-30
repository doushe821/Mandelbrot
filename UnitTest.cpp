#include "UnitTest.h"

#include "MandelbrotCalculation.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

const int MAX_DATA_STRING_LENGTH = 10;
enum TestExitCode UnitTest(const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius, const int TestNumber, const unsigned long long CPUfrequency, FILE* fpData, const char* DataFileName, FILE* fpInfo, FILE* fpPlotRaw, FILE* fpPlotOptimized)
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
    if(TestNumber == 0)
    {
        return ZERO_TEST_NUMBER;
    }
    if(CPUfrequency == 0)
    {
        return ZERO_CPU_FREQUENCY;
    }
    double* LatencyDataArrayRaw = (double*)calloc((size_t)TestNumber, sizeof(double));
    double* LatencyDataArrayOptimized = (double*)calloc((size_t)TestNumber, sizeof(double));
    unsigned long long ClocksRaw = 0;
    unsigned long long ClocksOptimized = 0;

    for(int i = 0; i < TestNumber; i++)
    {
        unsigned long long start = _rdtsc();
        int* test = MandelbrotRaw(ScreenX, ScreenY, ProbeNumber, step, CenterX, CenterY, BorderRadius);
        unsigned long long end = _rdtsc();

        unsigned long long DeltaClocks = end - start;
        LatencyDataArrayRaw[i] = (double)(DeltaClocks) / (double)CPUfrequency;
        ClocksRaw += DeltaClocks;

        free(test);

        start = _rdtsc();
        test = MandelbrotOptimized(ScreenX, ScreenY, ProbeNumber, step, CenterX, CenterY, BorderRadius);
        end = _rdtsc();

        DeltaClocks = end - start;
        LatencyDataArrayOptimized[i] = (double)(DeltaClocks) / (double)CPUfrequency;
        ClocksOptimized += DeltaClocks; 

        free(test);
    }

    double RawLatency = (double)ClocksRaw / (double)CPUfrequency;
    double OptimizedLatency = (double)ClocksOptimized / (double)CPUfrequency; 
    fprintf(fpInfo, "%s\nCPU frequency = %llu\nNumber of tests = %d\nShort summary:\nRaw latency = %lg; average raw latency = %lg\n"
    "Optimized latency = %lg; average optimized latency = %lg\nPerformance increase: %lg%%\n" , 
    __TIME__, CPUfrequency, TestNumber, RawLatency, RawLatency / TestNumber,
    OptimizedLatency, OptimizedLatency / TestNumber, (double)ClocksRaw / (double)ClocksOptimized);

    char* DataBuffer = (char*)calloc((size_t)(MAX_DATA_STRING_LENGTH) * (size_t)TestNumber * 2, sizeof(char*));
    int lastPrinted = 0;
    for(int i = 0; i < TestNumber; i++)
    {
        lastPrinted += sprintf(DataBuffer + lastPrinted, "%.6lg\n",LatencyDataArrayRaw[i]);
    }
    for(int i = 0; i < TestNumber; i++)
    {
        lastPrinted += sprintf(DataBuffer + lastPrinted, "%.6lg\n", LatencyDataArrayOptimized[i]);
    }

    fwrite(DataBuffer, sizeof(char), strlen(DataBuffer), fpData);

    free(DataBuffer);

    // PLOTTING 

    fprintf(fpPlotRaw, "import matplotlib.pyplot as plt\n\nwith open('%s', 'r') as f:\n"
    "   data = [float(line.strip()) for line in f]\n"
    "\n\nplt.hist(data[1:%d], bins = 20, color = 'blue', edgecolor = 'black', alpha = 0.7)\n"
    "plt.grid(axis='y', linestyle='--')\n"
    "plt.title('Latency distribution (raw)')\n"
    "plt.xlabel('Latency')\n"
    "plt.ylabel('Quantity')\n"
    "plt.savefig('histRaw.png', dpi = 300)", DataFileName, TestNumber/2);

    fprintf(fpPlotOptimized, "import matplotlib.pyplot as plt\n\nwith open('%s', 'r') as f:\n"
    "   data = [float(line.strip()) for line in f]\n"
    "\n\nplt.hist(data[%d:%d], bins = 20, color = 'blue', edgecolor = 'black', alpha = 0.7)\n"
    "plt.grid(axis='y', linestyle='--')\n"
    "plt.title('Latency distribution (optimized)')\n"
    "plt.xlabel('Latency')\n"
    "plt.ylabel('Quantity')\n"
    "plt.savefig('histOptimized.png', dpi = 300)", DataFileName, TestNumber/2 + 1, TestNumber);


    free(LatencyDataArrayOptimized);
    free(LatencyDataArrayRaw);

    return TEST_SUCCESS;
}
