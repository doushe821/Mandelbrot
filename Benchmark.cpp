#include "Benchmark.h"

#include "MandelbrotCalculation.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

const int MAX_DATA_STRING_LENGTH = 10;


enum ErrorCodes Benchmark(RunParameters params, FILE* fpData, FILE* fpInfo, FILE* fpPlotNaive, FILE* fpPlotOptimized, FILE* fpPlotArrays)
{
    if(fpData == NULL)
    {
        return FP_DATA_NULL;
    }   
    if(fpInfo == NULL)
    {
        return FP_INFO_NULL;
    }
    if(fpPlotNaive == NULL)
    {
        return FP_PLOT_NAIVE_NULL;
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
    double* LatencyDataArrayNaive = (double*)calloc((size_t)params.TestNumber, sizeof(double));
    if(LatencyDataArrayNaive == NULL)
    {
        return ALLOCATION_FAILURE;
    }
    double* LatencyDataArrayOptimized = (double*)calloc((size_t)params.TestNumber, sizeof(double));
    if(LatencyDataArrayOptimized == NULL)
    {
        free(LatencyDataArrayNaive);
        return ALLOCATION_FAILURE;
    }
    double* LatencyDataArrayArrays = (double*)calloc((size_t)params.TestNumber, sizeof(double));
    if(LatencyDataArrayArrays == NULL)
    {
        free(LatencyDataArrayArrays);
        return ALLOCATION_FAILURE;  
    }

    unsigned long long ClocksNaive = 0;
    unsigned long long ClocksOptimized = 0;
    unsigned long long ClocksArrays = 0;

    size_t ScreenArea = (size_t)params.ScreenX * (size_t)params.ScreenY;
    if((ScreenArea % sizeof(__m256i)) != 0)
    {
        ScreenArea += sizeof(__m256i) - ScreenArea % sizeof(__m256i);
    }
    int32_t* PixelSet = (int*)aligned_alloc(sizeof(__m256i), (size_t)params.ScreenX * (size_t)params.ScreenY * sizeof(int));
    
    if(PixelSet == NULL)
    {
        free(LatencyDataArrayArrays);
        free(LatencyDataArrayOptimized);
        free(LatencyDataArrayNaive);
    
        return ALLOCATION_FAILURE;
    }

    unsigned long long start = 0;
    unsigned long long end = 0;

    u_int32_t TSC_AUX_VALUE_STORAGE = 0;
    for(int i = 0; i < params.TestNumber; i++)
    {
        start = __rdtscp(&TSC_AUX_VALUE_STORAGE); 
        MandelbrotNaive(PixelSet, params.ScreenX, params.ScreenY, params.ProbeNumber, params.step, params.CenterX, params.CenterY, params.BorderRadius);
        end = _rdtsc();

        unsigned long long DeltaClocks = end - start;
        LatencyDataArrayNaive[i] = (double)(DeltaClocks) / (double)params.CPUFrequency;
        ClocksNaive += DeltaClocks;

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

    double NaiveLatency = (double)ClocksNaive / (double)params.CPUFrequency;
    double OptimizedLatency = (double)ClocksOptimized / (double)params.CPUFrequency; 
    double ArraysLatency = (double)ClocksArrays / (double)params.CPUFrequency; 

    fprintf(fpInfo, "%s\nCPU frequency = %llu\nNumber of tests = %d\nNumber of probes for each dot: %d\nShort summary:\nNaive latency = %lg; average naive latency = %lg\n"
    "Optimized latency = %lg; average optimized latency = %lg\nRelative Performance increase: %lg\nArrays latency: %lg\n" , 
    __TIME__, params.CPUFrequency, params.TestNumber, params.ProbeNumber, NaiveLatency, NaiveLatency / params.TestNumber,
    OptimizedLatency, OptimizedLatency / params.TestNumber, (double)ClocksNaive / (double)ClocksOptimized, ArraysLatency);

    char* DataBuffer = (char*)calloc((size_t)(MAX_DATA_STRING_LENGTH) * (size_t)params.TestNumber * 3, sizeof(char*));
    if(DataBuffer == NULL)
    {
        free(LatencyDataArrayArrays);
        free(LatencyDataArrayNaive);
        free(LatencyDataArrayOptimized);
        return ALLOCATION_FAILURE;
    }

    int lastPrinted = 0;
    for(int i = 0; i < params.TestNumber; i++)
    {
        lastPrinted += sprintf(DataBuffer + lastPrinted, "%.6lg\n",LatencyDataArrayNaive[i]);
    }
    for(int i = 0; i < params.TestNumber; i++)
    {
        lastPrinted += sprintf(DataBuffer + lastPrinted, "%.6lg\n", LatencyDataArrayOptimized[i]);
    }
    for(int i = 0; i < params.TestNumber; i++)
    {
        lastPrinted += sprintf(DataBuffer + lastPrinted, "%.6lg\n", LatencyDataArrayArrays[i]);
    }

    fwrite(DataBuffer, sizeof(char), strlen(DataBuffer), fpData);

    free(DataBuffer);

    fprintf(fpPlotNaive, "import matplotlib.pyplot as plt\nimport numpy as np\n\nwith open('%s.dat', 'r') as f:\n"
    "   data = [float(line.strip()) for line in f]\n"
    "average = np.sum(data[1:%d])/%d\n"
    "disp = np.sum((average - data[1:%d])**2)\n"
    "sigma = np.sqrt(disp / (%d * %d))\n"
    "print(sigma)\n"
    "\n\nplt.hist(data[1:%d], bins = 30, color = 'blue', edgecolor = 'black', alpha = 0.7)\n"
    "plt.grid(axis='y', linestyle='--')\n"
    "plt.title('Latency distribution (naive)')\n"
    "plt.xlabel('Times latency appeared')\n"
    "plt.ylabel('Quantity')\n"
    "plt.savefig('%s.png', dpi = 300)", params.DataFname, params.TestNumber, params.ProbeNumber, params.TestNumber, params.ProbeNumber, params.ProbeNumber - 1, params.TestNumber, params.NaivePlotFname);


    fprintf(fpPlotOptimized, "import matplotlib.pyplot as plt\nimport numpy as np\n\nwith open('%s.dat', 'r') as f:\n"
    "   data = [float(line.strip()) for line in f]\n"
    "average = np.sum(data[%d:%d])/%d\n"
    "disp = np.sum((average - data[%d:%d])**2)\n"
    "sigma = np.sqrt(disp / (%d * %d))\n"
    "print(sigma)\n"    
    "\n\nplt.hist(data[%d:%d], bins = 30, color = 'blue', edgecolor = 'black', alpha = 0.7)\n"
    "plt.grid(axis='y', linestyle='--')\n"
    "plt.title('Latency distribution (optimized)')\n"
    "plt.xlabel('Latency')\n"
    "plt.ylabel('Times latency appeared')\n"
    "plt.savefig('%s.png', dpi = 300)", params.DataFname, params.TestNumber + 1, params.TestNumber * 2, params.ProbeNumber, params.TestNumber + 1, params.TestNumber * 2, params.ProbeNumber, params.ProbeNumber - 1, params.TestNumber + 1, 2 * params.TestNumber, params.OptimizedPlotFname);


    fprintf(fpPlotArrays, "import matplotlib.pyplot as plt\nimport numpy as np\n\nwith open('%s.dat', 'r') as f:\n"
        "   data = [float(line.strip()) for line in f]\n"
        "average = np.sum(data[%d:%d])/%d\n"
        "disp = np.sum((average - data[%d:%d])**2)\n"
        "sigma = np.sqrt(disp / (%d * %d))\n"
        "print(sigma)\n"    
        "\n\nplt.hist(data[%d:%d], bins = 30, color = 'blue', edgecolor = 'black', alpha = 0.7)\n"
        "plt.grid(axis='y', linestyle='--')\n"
        "plt.title('Latency distribution (arrays)')\n"
        "plt.xlabel('Latency')\n"
        "plt.ylabel('Times latency appeared')\n"
        "plt.savefig('%s.png', dpi = 300)", params.DataFname, 2* params.TestNumber + 1, params.TestNumber * 3, params.ProbeNumber, 2* params.TestNumber + 1, params.TestNumber * 3, params.ProbeNumber, 2 * params.ProbeNumber - 1, params.TestNumber + 1, 3 * params.TestNumber, params.ArraysPlotFname);
    
   
    free(LatencyDataArrayOptimized);
    free(LatencyDataArrayNaive);
    free(LatencyDataArrayArrays);

    return MODULE_SUCCESS;
}
