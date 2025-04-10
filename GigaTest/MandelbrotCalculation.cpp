#include "MandelbrotCalculation.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

#include <emmintrin.h>
#include <xmmintrin.h>

int* MandelbrotRaw(int* PixelSet, const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius)
{
    for(int yPixels = 0; yPixels < ScreenY; yPixels++)
    {
        float Y = ((float)(CenterY - yPixels)) * step;
        float y0 = Y;
        for(int xPixels = 0; xPixels < ScreenX; xPixels++)
        {
            float X = ((float)(xPixels - CenterX)) * step;
            Y = ((float)(CenterY - yPixels)) * step;
            float x0 = X;
            int Iterations = 0;
            for(; Iterations < ProbeNumber; Iterations++)
            {
                float xx = X*X;
                float yy = Y*Y;
                float xy = X*Y;

                if(xx + yy > BorderRadius)
                {
                    break;
                }

                X = xx - yy + x0;
                Y = 2 * xy + y0;
            }
            PixelSet[yPixels * ScreenX + xPixels] = Iterations;
        }
    }
    return PixelSet;
}

int* MandelbrotOptimized(int* PixelSet, const int ScreenX, const int ScreenY, const int ProbeNumber, float step, int CenterX, int CenterY, const float BorderRadius)
{
    __m256 xSquare = {};
    __m256 ySquare = {};
    __m256 xy = {};
    __m256 xInitial = {};
    __m256 yInitial = {};
    __m256 xCurrent = {};
    __m256 yCurrent = {};

    __m256 CenterX_m256 = _mm256_set1_ps((float)CenterX);
    __m256 CenterY_m256 = _mm256_set1_ps((float)CenterY);
    __m256 step_m256 = _mm256_set1_ps(step);

    __m256 Const2_m256 = _mm256_set1_ps(2);
    __m256 BorderRadius_m256 = _mm256_set1_ps(BorderRadius);
    __m256 ZeroToThreeConstants_m256 = _mm256_set_ps(7, 6, 5, 4, 3, 2, 1, 0);

    for(int yPixels = 0; yPixels < ScreenY; yPixels++)
    {
        yInitial = _mm256_set1_ps((float)yPixels);
        yInitial = _mm256_sub_ps(CenterY_m256, yInitial);
        yInitial = _mm256_mul_ps(yInitial, step_m256);
        
        for(int xPixels = 0; xPixels < ScreenX; xPixels += 8)
        {
            xCurrent = _mm256_set1_ps((float)xPixels);
            xCurrent = _mm256_add_ps(xCurrent, ZeroToThreeConstants_m256);
            xCurrent = _mm256_sub_ps(xCurrent, CenterX_m256);
            xCurrent = _mm256_mul_ps(xCurrent, step_m256);

            yCurrent = yInitial;
            xInitial = xCurrent;

            __m256i ProbeQuantity = {};
            for(int i = 0; i < ProbeNumber; i++)
            {
                // x^2
                xSquare = _mm256_mul_ps(xCurrent, xCurrent);
                // y^2
                ySquare = _mm256_mul_ps(yCurrent, yCurrent);
                // x * y
                xy = _mm256_mul_ps(xCurrent, yCurrent);

                // x^2 + y^2 < radius
                __m256 Distance = _mm256_add_ps(xSquare, ySquare);
                __m256 CmpResult = _mm256_cmp_ps(Distance, BorderRadius_m256, _CMP_LT_OS);
                int mask = _mm256_movemask_ps(CmpResult);

                if(mask == 0)
                {
                    break;
                }

                // Increasing ProbeQuantity according to mask
                ProbeQuantity = _mm256_sub_epi32(ProbeQuantity, *((__m256i*)(&CmpResult)));

                // x_n+1 = x^2_n - y^2_n + x_0
                xCurrent = _mm256_sub_ps(xSquare, ySquare);
                xCurrent = _mm256_add_ps(xCurrent, xInitial);

                // y_n+1 = 2 * x_n * y_n + y_0
                xy = _mm256_mul_ps(xy, Const2_m256);
                yCurrent = _mm256_add_ps(xy, yInitial);
            }

            // Copying N values to PixelSet
            //*((__m256i*)(PixelSet + yPixels * ScreenX + xPixels)) = *((__m256i*)(ProbeQuantity));
            memcpy(PixelSet + yPixels * ScreenX + xPixels, &ProbeQuantity, sizeof(int) * 8);
        }
    }
    return PixelSet;   
}

// © Dr. Iyaitsa

// "Где я..."
// © Pr. Penis