#include "Visualizer.h"
#include "ErrorParser.h"
#include "MandelbrotCalculation.h"

#include <SDL2/SDL.h>

enum ErrorCodes DisplayPixelsSDL(const int ScreenX, const int ScreenY, const int MaxN)
{
    float step = 0.003f;
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL initialisation error: %s\n", SDL_GetError());
        SDL_Quit();
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

    const float BorderRadius = 3;
    const int MoveSpeed = 5;
    const float ZoomSpeed = 0.0001f;
    alignas(__m128i) int* PixelSet = (int*)calloc((size_t)(ScreenX * ScreenY), sizeof(int));
    alignas(__m128i) int* PixelsRGB = (int*)calloc((size_t)(ScreenX * ScreenY), sizeof(int));
    int CenterX = ScreenX / 2;
    int CenterY = ScreenY / 2;

    int UpdateFlag = 1;

    if(PixelSet == NULL)
    {
        SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return VISUALIZER_ARRAY_ALLOCATION_FAILURE;
    }

    int RunFlag = 1;
    SDL_Event Event = {};
    while(RunFlag)
    {
        while(SDL_PollEvent(&Event))
        {
            //if(Event.type == SDL_QUIT)
            //{
            //    RunFlag = 0;
            //}
            switch(Event.type)
            {
                case SDL_QUIT:
                {
                    RunFlag = 0;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    switch(Event.key.keysym.sym)
                    {
                        case SDLK_a:
                        {
                            CenterX += MoveSpeed;
                            UpdateFlag = 1;
                            break;
                        }
                        case SDLK_d:
                        {
                            CenterX -= MoveSpeed;
                            UpdateFlag = 1;
                            break;
                        }
                        case SDLK_w:
                        {
                            CenterY += MoveSpeed;
                            UpdateFlag = 1;
                            break;
                        }
                        case SDLK_s:
                        {
                            CenterY -= MoveSpeed;
                            UpdateFlag = 1;
                            break;
                        }
                        case SDLK_EQUALS:   
                        {
                            step -= ZoomSpeed;
                            float DeltaCenter = (float)CenterX * (step + ZoomSpeed) - (float)CenterX * (step);
                            CenterX -= (int)((float)DeltaCenter / step);
                            DeltaCenter = (float)CenterY * (step + ZoomSpeed) - (float)CenterY * (step);
                            CenterY -= (int)((float)DeltaCenter / step);
                            UpdateFlag = 1;
                            break;
                        }
                        case SDLK_MINUS:
                        {
                            step += ZoomSpeed; 
                            float DeltaCenter = (float)CenterY * (step + ZoomSpeed) - (float)CenterY * (step);
                            CenterY -= (int)((float)DeltaCenter / step);
                            //CenterY += CenterY * step;
                            UpdateFlag = 1;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    default:
                    {
                        break;
                    }
                }
            }

            void* TextureBuffer = {};
            int pitch = 0;

            SDL_LockTexture(texture, NULL, &TextureBuffer, &pitch);

            if(UpdateFlag)
            {
                MandelbrotOptimized(PixelSet, ScreenX, ScreenY, MaxN, step, CenterX, CenterY, BorderRadius);
            }    
            for(long int i = 0; i < ScreenX * ScreenY; i++)
            {
                Uint8 red = ((Uint8)((Uint8)MaxN - (Uint8)PixelSet[i]) % 255);
                Uint8 green = (Uint8)((Uint8)MaxN - (Uint8)PixelSet[i]) % 255;
                Uint8 blue = (Uint8)((Uint8)MaxN - (Uint8)PixelSet[i]) % 255;
                PixelsRGB[i] = red + (green << 8) + (blue << 16) + (255 << 24);
            }    
            for(int i = 0; i < ScreenY; i++)
            {
                int* dstRow = (int*)((Uint8*)TextureBuffer + i * pitch);
                memcpy(dstRow, &PixelsRGB[i * ScreenX], (size_t)ScreenX * sizeof(Uint32));   
            }
            //SDL_UpdateTexture(texture, NULL, PixelSet, (size_t)ScreenX * sizeof(int));
            SDL_UnlockTexture(texture);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);    
            UpdateFlag = 0;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    SDL_Quit();
    free(PixelSet);
    free(PixelsRGB);
    return MODULE_SUCCESS;
}