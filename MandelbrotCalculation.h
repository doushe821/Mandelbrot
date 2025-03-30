#ifndef MANDELBROT_CALCULATION_H_INCLUDED
#define MANDELBROT_CALCULATION_H_INCLUDED

int* MandelbrotOptimized(int* PixelSet, const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius);
int* MandelbrotRaw(int* PixelSet, const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius);

#endif // MANDELBROT_CALCULATION_H_INCLUDED