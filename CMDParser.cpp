
#include <string.h>

#include "CMDParser.h"

static char* itoa(int num, char* string);

enum ErrorCodes ParseCMD(int argc, char** argv, flags_t* flags)
{
    if(argc < 2)
    {
        return MODULE_SUCCESS;
    }

    int opt = 0;
    int optid = 0;
    for(int i = 0; i < argc - 1; i++)
    {
        if((opt = getopt_long(argc, argv, "", options, &optid)) != -1)
        {
            switch(opt)
            {
                case 'h':
                {
                    flags->isHelp = true;
                    break;
                }
                case 'd':
                {
                    flags->isDimensions = true;
                    flags->ScreenX = atoi(optarg);
                    flags->ScreenY = atoi(optarg + strlen(optarg) + 1);
                    break;
                }
                case 'n':
                {
                    flags->isProbeNumber = true;
                    flags->ProbeNumber = atoi(optarg);

                    break;
                }
                case 't':
                {
                    flags->isTest = true;
                    flags->TestNumber = atoi(optarg);

                    break;
                }
                case 'g':
                {
                    flags->isGraphic = true;

                    break;
                }
                case 's':
                {
                    flags->isStep = true;
                    flags->step = (float)atof(optarg);
                    break;
                }
                case 'f':
                {
                    flags->CPUFrequency = (unsigned long long)atoll(optarg);
                    break;
                }
                case '?':
                {
                    fprintf(stderr, "%s", UNKNOWN_COMMAND_MESSAGE);
                    return PARSER_UNKNOWN_COMMAND;
                }
                case ':':
                {
                    fprintf(stderr, "%s", MISSING_ARGUMENT);
                    return PARSER_MISSING_ARGUMENT;
                }
                default:
                {
                    fprintf(stderr, "%s", PARSER_FAILURE);
                    return UNKNOWN_PARSER_FAILURE;
                }
            }
        }
    }
    return MODULE_SUCCESS;
}


enum ErrorCodes InitParams(flags_t CMDFlags, RunParameters* params)
{
    const int ScreenXdefault = 800;
    const int ScreenYdefault = 800;
    const float stepDefault = 0.003f;
    const float BorderRadiusDefault = 3;
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

    if(CMDFlags.isStep)
    {
        params->step = CMDFlags.step;
    }
    else
    {
        params->step = stepDefault;
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

    params->CenterX = params->ScreenX / 2;
    params->CenterY = params->ScreenY / 2;

    params->GraphicsFlag = CMDFlags.isGraphic;


    return MODULE_SUCCESS;
}

enum ErrorCodes FileNamesInit(RunParameters* params)
{
    char txtExt[5] = ".txt";
    char pyExt[4] = ".py";
    char datExt[5] = ".dat"; 
    char InfoFname[MANDELBROT_FILENAME_MAX] = "info";
    char DataFname[MANDELBROT_FILENAME_MAX] = "data";

    char DataFP[MANDELBROT_FILENAME_MAX] = "Plots/";
    char InfoFP[MANDELBROT_FILENAME_MAX] = "Plots/";

    char pnString[MAX_PROBE_NUMBER_DIGITS] = {};
    itoa(params->ProbeNumber, pnString);

    char NaiveFname[MANDELBROT_FILENAME_MAX] = "histNaive";
    char OptFname[MANDELBROT_FILENAME_MAX] = "histOptimized";
    char ArraysFname[MANDELBROT_FILENAME_MAX] = "histArrays";

    char NaiveFP[MANDELBROT_FILENAME_MAX] = "Plots/";
    char OptFP[MANDELBROT_FILENAME_MAX] = "Plots/";
    char ArraysFP[MANDELBROT_FILENAME_MAX] = "Plots/";



    strcat(InfoFname, pnString);
    strcat(DataFname, pnString);

    strncpy(params->InfoFname, InfoFname, MANDELBROT_FILENAME_MAX);
    strncpy(params->DataFname, DataFname, MANDELBROT_FILENAME_MAX);

    strcat(DataFP, DataFname);
    strcat(InfoFP, InfoFname);
    strcat(DataFP, datExt);
    strcat(InfoFP, txtExt);

    strncpy(params->DataFnameFullPath, DataFP, MANDELBROT_FILENAME_MAX);
    strncpy(params->InfoFnameFullPath, InfoFP, MANDELBROT_FILENAME_MAX);

    strcat(NaiveFP, NaiveFname);
    strcat(NaiveFP, pnString);
    strcat(NaiveFP, pyExt);

    strcat(OptFP, OptFname);
    strcat(OptFP, pnString);
    strcat(OptFP, pyExt);

    strcat(ArraysFP, ArraysFname);
    strcat(ArraysFP, pnString);
    strcat(ArraysFP, pyExt); 

    strncpy(params->OptimizedPyFname, OptFP, MANDELBROT_FILENAME_MAX);
    strncpy(params->NaivePyFname, NaiveFP, MANDELBROT_FILENAME_MAX);
    strncpy(params->ArraysPyFname, ArraysFP, MANDELBROT_FILENAME_MAX);

    strcat(NaiveFname, pnString);
    strcat(OptFname, pnString);
    strcat(ArraysFname, pnString);


    strncpy(params->NaivePlotFname, NaiveFname, MANDELBROT_FILENAME_MAX);
    strncpy(params->OptimizedPlotFname, OptFname, MANDELBROT_FILENAME_MAX);
    strncpy(params->ArraysPlotFname, ArraysFname, MANDELBROT_FILENAME_MAX);

    return MODULE_SUCCESS;

}

static char* itoa(int num, char* string)
{
    sprintf(string, "%d", num);
    return string;
}