#ifndef MANDELBROT_CALCULATION_H_INCLUDED
#define MANDELBROT_CALCULATION_H_INCLUDED

#include "ErrorParser.h"

enum ErrorCodes MandelbrotIntrinsics(int* PixelSet, const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius);
enum ErrorCodes MandelbrotNaive(int* PixelSet, const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius);
enum ErrorCodes MandelbrotArrays(int* PixelSet, const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius);

#endif // MANDELBROT_CALCULATION_H_INCLUDED