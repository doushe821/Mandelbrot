#include "Visualizer.h"
#include "ErrorParser.h"

#include <SDL2/SDL.h>

int DisplayPixelsSDL(const int ScreenX, const int ScreenY, const int MaxN)
{
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

  //  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, ScreenX, ScreenY);
  //  if(!texture)
  //  {
  //      SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
  //      SDL_DestroyTexture(texture);
  //      SDL_DestroyWindow(window);
  //      SDL_Quit();
  //      return SDL_TEXTURE_CREATION_FAILURE; 
  //  }

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
                SDL_SetRenderDrawColor(renderer, 255 - ((Uint8)PixelSet[i] % 255) - (255 - (Uint8)MaxN), (255 - (Uint8)PixelSet[i] % 255)- (255 - (Uint8)MaxN), (255 - (Uint8)PixelSet[i] % 255)- (255 - (Uint8)MaxN), 255);
                SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            }
            SDL_RenderPresent(renderer);    
        }
    }

    //SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    SDL_Quit();

    return 0;
}