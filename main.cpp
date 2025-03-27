#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <emmintrin.h>
#include <xmmintrin.h>

#include <SDL2/SDL.h>

float CalculateDistanceDecartes(const float x1, const float y1, const float x0, const float y0);
int floatCmp(const float a, const float b, const float epsilon);
int ScreenCenterCoorinates(const int ScreenX, const int ScreenY);
void ErrorParser(const int ErrorCode);
int DisplayPixelsSDL(const int* PixelSet, const int ScreenX, const int ScreenY);
int Mandelbrot(int GraphicModeFlag);
int MandelbrotIntrinsics(int GraphicModeFlag);

int MandelbrotRaw(int GraphicModeFlag);
enum ErrorCodes
{
    SDL_INITIALISATION_ERROR = -10,
    SDL_WINDOW_CREATION_FAILURE,
    SDL_RENDERER_CREATION_FAILURE,
    SDL_TEXTURE_CREATION_FAILURE,
};

int main()
{


    int tests = 1;

    MandelbrotIntrinsics(0);
    for(int i = 0; i < tests; i++)
    {
        Mandelbrot(0);
    }

    for(int i = 0; i < tests; i++)
    {
        MandelbrotRaw(0);
    }
    return 0;
}
int MandelbrotRaw(int GraphicModeFlag)
{
    clock_t start, end;
    start = clock();
    const int ScreenX = 800;
    const int ScreenY = 800;
    int PixelSet[ScreenX * ScreenY] = {};
    const int N = 200;
    const float stepX = 0.003f;
    const float stepY = 0.003f;
    const float BorderCircumferenceRadiusSquare = 3;

    const int CenterX = ScreenX/2;
    const int CenterY = ScreenY/2;
    
    for(int yPixels = 0; yPixels < ScreenY; yPixels++)
    {
        float Y = ((float)(CenterY - yPixels)) * stepY;
        float y0 = Y;
        for(int xPixels = 0; xPixels < ScreenX; xPixels++)
        {
            float X = ((float)(xPixels - CenterX)) * stepX;
            Y = ((float)(CenterY - yPixels)) * stepY;
            float x0 = X;
            int Iterations = 0;
            for(; Iterations < N; Iterations++)
            {
                float xx = X*X;
                float yy = Y*Y;
                float xy = X*Y;

                if(xx + yy > BorderCircumferenceRadiusSquare)
                {
                    break;
                }

                X = xx - yy + x0;
                Y = 2 * xy + y0;
            }
            PixelSet[yPixels * ScreenX + xPixels] = Iterations;
        }
    }
    end = clock();
    printf("FPS: %lg\n", 1 / ((((float)(end - start)) / CLOCKS_PER_SEC)));
    if(GraphicModeFlag)
    {
        ErrorParser(DisplayPixelsSDL(PixelSet, ScreenX, ScreenY));
    }
    return 0;
}

int MandelbrotIntrinsics(int GraphicModeFlag)
{
    const int ScreenX = 800;
    const int ScreenY = 800;
    int PixelSet[ScreenX * ScreenY] = {};
    const int N = 200;
    const float stepX = 0.003f;
    const float stepY = 0.003f;
    const float BorderCircumferenceRadiusSquare = 3;

    const int CenterX = ScreenX/2;
    const int CenterY = ScreenY/2;
    
    __m128 yy = {};
    __m128 xx = {};
    __m128 xy = {};
    __m128 X_0 = {};
    __m128 YValues = {};
    __m128 Y_0 = {};
    __m128 BorderRadius = {4, 4, 4, 4};

    clock_t start, end;
    start = clock();
    for(int yPixels = 0; yPixels < ScreenY; yPixels += 1)
    {
        float y0 = ((float)(CenterY - yPixels)) * stepY;

        for(int i = 0; i <= 3; i++)
        {
            Y_0[i] = y0;
        }

        for(int xPixels = 0; xPixels < ScreenX; xPixels += 4)
        {
            __m128 XValues = {((float)(xPixels - CenterX)) * stepX, ((float)(xPixels + 1 - CenterX)) * stepX, ((float)(xPixels + 2 - CenterX)) * stepX, ((float)(xPixels + 3 - CenterX)) * stepX};

            for(int i = 0; i <=3; i++)
            {
                YValues[i] = y0;
            }
            X_0 = XValues;

            int NValues[4] = {0};
            int ValueMask[4] = {1, 1, 1, 1};
            for(int Iterations = 0; Iterations < N; Iterations++)
            {
                xx = _mm_mul_ps(XValues, XValues);
                yy = _mm_mul_ps(YValues, YValues);
                xy = _mm_mul_ps(XValues, YValues);

                for(int i = 0; i < 4; i++)
                {
                    if(xx[i] + yy[i] > BorderCircumferenceRadiusSquare)
                    {
                        ValueMask[i] = 0;
                    }
                }
                for(int i = 0; i < 4; i++)
                {
                    NValues[i] += ValueMask[i];
                }
                XValues = _mm_add_ps(_mm_sub_ps(xx, yy), X_0);
                YValues = _mm_add_ps(2 * xy, Y_0);
            }
            for(int i = 0; i < 4; i++)
            {
                PixelSet[yPixels * ScreenX + xPixels + i] = NValues[i];
            }
        }
    }
    end = clock();
    printf("FPS: %lg\n", 1 / ((((float)(end - start)) / CLOCKS_PER_SEC)));
    if(GraphicModeFlag)
    {
        ErrorParser(DisplayPixelsSDL(PixelSet, ScreenX, ScreenY));
    }

    return 0;
}

int Mandelbrot(int GraphicModeFlag)
{
    const int ScreenX = 800;
    const int ScreenY = 800;
    int PixelSet[ScreenX * ScreenY] = {};
    const int N = 200;
    const float stepX = 0.003f;
    const float stepY = 0.003f;
    const float BorderCircumferenceRadiusSquare = 3;

    const int CenterX = ScreenX/2;
    const int CenterY = ScreenY/2;
    
    float yy[4] = {};
    float xx[4] = {};
    float xy[4] = {};
    float ZeroX[4] = {};
    float XValues[4] = {};
    float YValues[4] = {};
    float Zeroy[4] = {};

    clock_t start, end;
    start = clock();
    for(int yPixels = 0; yPixels < ScreenY; yPixels += 1)
    {
        float y0 = ((float)(CenterY - yPixels)) * stepY;

        for(int i = 0; i < 4; i++) YValues[i] = y0;

        for(int i = 0; i < 4; i++) Zeroy[i] = y0;

        for(int xPixels = 0; xPixels < ScreenX; xPixels += 4)
        {
            for(int i = 0; i < 4; i++) XValues[i] = ((float)(xPixels + i - CenterX)) * stepX;

            for(int i = 0; i < 4; i++) YValues[i] = y0;

            for(int i = 0; i < 4; i++) ZeroX[i] = XValues[i];

            int NValues[4] = {0};
            int ValueMask[4] = {1, 1, 1, 1};
            for(int Iterations = 0; Iterations < N; Iterations++)
            {
                for(int i = 0; i < 4; i++) xx[i] = XValues[i] * XValues[i];
                for(int i = 0; i < 4; i++) yy[i] = YValues[i] * YValues[i];
                for(int i = 0; i < 4; i++) xy[i] = XValues[i] * YValues[i];

                for(int i = 0; i < 4; i++)
                {
                    if(xx[i] + yy[i] > BorderCircumferenceRadiusSquare)
                    {
                        ValueMask[i] = 0;
                    }
                }

                for(int i = 0; i < 4; i++)
                {
                    XValues[i] = xx[i] - yy[i] + ZeroX[i];
                    YValues[i] = 2 * xy[i] + Zeroy[i];
                    NValues[i] += ValueMask[i];
                }

            }
            for(int i = 0; i < 4; i++)
            {
                PixelSet[yPixels * ScreenX + xPixels + i] = NValues[i];
            }
        }
    }
    end = clock();
    printf("FPS: %lg\n", 1 / ((((float)(end - start)) / CLOCKS_PER_SEC)));
    if(GraphicModeFlag)
    {
        ErrorParser(DisplayPixelsSDL(PixelSet, ScreenX, ScreenY));
    }

    return 0;
}

//mm_sub_ps(float* )
//{
//x1
//}


int DisplayPixelsSDL(const int* PixelSet, const int ScreenX, const int ScreenY)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL initialisation error: %s\n", SDL_GetError());
        return SDL_INITIALISATION_ERROR;
    }

    SDL_Window *window = SDL_CreateWindow(
    "Mandelbrot set",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    (int)ScreenX, (int)ScreenY,
    SDL_WINDOW_SHOWN);

    if(!window)
    {
        SDL_Log("Failed to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return SDL_WINDOW_CREATION_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_RENDERER_CREATION_FAILURE;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, ScreenX, ScreenY);
    if(!texture)
    {
        SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_TEXTURE_CREATION_FAILURE; 
    }

    int RunFlag = 1;
    SDL_Event Event = {};
    while(RunFlag)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                RunFlag = 0;
            }
            
            //SDL_LockTexture(texture, );
            //SDL_UpdateTexture(texture, NULL, PixelSet, (size_t)ScreenX * sizeof(*PixelSet));
            //SDL_RenderClear(renderer);
            //SDL_RenderCopy(renderer, texture, NULL, NULL);
            //SDL_RenderPresent(renderer);

            for(long int i = 0; i < ScreenX * ScreenY; i++)
            {
                long int x = i % ScreenX;
                long int y = i / ScreenX; 
                SDL_SetRenderDrawColor(renderer, 255 - ((Uint8)PixelSet[i] % 255), (255 - (Uint8)PixelSet[i] % 255), (255 - (Uint8)PixelSet[i] % 255), 255);
                SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            }
            SDL_RenderPresent(renderer);    
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void ErrorParser(const int ErrorCode) // TODO
{
    switch(ErrorCode)
    {
        default:
        {
            printf("MEOW!\n");
            break;
        }
    }


}

int floatCmp(const float a, const float b, const float epsilon)
{
    float c = a - b;
    if(c < epsilon && c > -epsilon)
    {
        return 0;
    }
    else if(c < -epsilon)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

float CalculateDistanceDecartes(const float x1, const float y1, const float x0, const float y0)
{
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

