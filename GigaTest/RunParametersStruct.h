#ifndef RUN_PARAMETERS_STRUCT_H_INCLUDED
#define RUN_PARAMETERS_STRUCT_H_INCLUDED

const size_t MAX_PROBE_NUMBER_DIGITS = 7;
const size_t MANDELBROT_FILENAME_MAX = 64;

struct RunParameters 
{
    int ScreenX;
    int ScreenY;
    int CenterX;
    int CenterY;
    float step;

    int ProbeNumber;

    float BorderRadius;

    unsigned long long CPUFrequency;
    int TestNumber;

    bool GraphicsFlag;

    char RawPyFname[MANDELBROT_FILENAME_MAX];
    char RawPlotFname[MANDELBROT_FILENAME_MAX];
    char OptimizedPyFname[MANDELBROT_FILENAME_MAX];
    char OptimizedPlotFname[MANDELBROT_FILENAME_MAX];

    char InfoFname[MANDELBROT_FILENAME_MAX];
    char DataFname[MANDELBROT_FILENAME_MAX];
    char InfoFnameFullPath[MANDELBROT_FILENAME_MAX];
    char DataFnameFullPath[MANDELBROT_FILENAME_MAX];
};

#endif