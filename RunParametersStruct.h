#ifndef RUN_PARAMETERS_STRUCT_H_INCLUDED
#define RUN_PARAMETERS_STRUCT_H_INCLUDED

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
};

#endif