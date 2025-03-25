#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>

double CalculateDistanceDecartes(const double x1, const double y1, const double x0, const double y0);
int DoubleCmp(const double a, const double b, const double epsilon);
int ScreenCenterCoorinates(const int ScreenX, const int ScreenY);
void ErrorParser(const int ErrorCode);
int DisplayPixelsSDL(const int* PixelSet, const int ScreenX, const int ScreenY);

enum ErrorCodes
{
    SDL_INITIALISATION_ERROR = -10,
    SDL_WINDOW_CREATION_FAILURE,
    SDL_RENDERER_CREATION_FAILURE,
    SDL_TEXTURE_CREATION_FAILURE,
};

int main()
{
    const int ScreenX = 800;
    const int ScreenY = 800;
    int PixelSet[ScreenX * ScreenY] = {};
    const int N = 255;
    const double stepX = 0.003;
    const double stepY = 0.003;
    const double BorderCircumferenceRadiusSquare = 3;

    const int CenterX = ScreenX/2;
    const int CenterY = ScreenY/2;
    
    for(int yPixels = 0; yPixels < ScreenY; yPixels++)
    {
        double Y = ((double)(CenterY - yPixels)) * stepY;
        double y0 = Y;
        for(int xPixels = 0; xPixels < ScreenX; xPixels++)
        {
            double X = ((double)(xPixels - CenterX)) * stepX;
            Y = ((double)(CenterY - yPixels)) * stepY;
            double x0 = X;
            int Iterations = 0;
            for(; Iterations < N; Iterations++)
            {
                double xx = X*X;
                double yy = Y*Y;
                double xy = X*Y;

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

    FILE* DebugFile = fopen("dbf.txt", "w+b");
    for(unsigned i = 0; i < ScreenX * ScreenY; i++)
    {
        fprintf(DebugFile, "%d ", PixelSet[i]);
    }
    fclose(DebugFile);
    ErrorParser(DisplayPixelsSDL(PixelSet, ScreenX, ScreenY));

    return 0;
}

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
                SDL_SetRenderDrawColor(renderer, 255 - (Uint8)PixelSet[i], 255 - (Uint8)PixelSet[i], 255 - (Uint8)PixelSet[i], 255);
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

int DoubleCmp(const double a, const double b, const double epsilon)
{
    double c = a - b;
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

double CalculateDistanceDecartes(const double x1, const double y1, const double x0, const double y0)
{
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

