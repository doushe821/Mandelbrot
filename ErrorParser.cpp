#include "ErrorParser.h"

#include <stdio.h>
void ErrorParser(const int ErrorCode)
{
    switch(ErrorCode)
    {
        case SDL_INITIALISATION_ERROR:
        {
            fprintf(stderr, "Failed to initialise SDL, skipping graphics.\n");
            break;
        }
        case SDL_WINDOW_CREATION_FAILURE:
        {
            fprintf(stderr, "Failed to create SDL window, skipping graphics\n");
            break;
        }
        case SDL_RENDERER_CREATION_FAILURE:
        {
            fprintf(stderr, "Failed to create SDL renderer, skipping graphics\n");
            break; 
        }
        case SDL_TEXTURE_CREATION_FAILURE:
        {
            fprintf(stderr, "Failed to create SDL texture, skipping graphics\n");
            break; 
        }
        case VISUALIZER_ARRAY_ALLOCATION_FAILURE:
        {
            fprintf(stderr, "Failed to allocate memory for visualizer, skipping graphics\n");
            break; 
        }
        case INCORRECT_DIMENSION_VALUE:
        {
            fprintf(stderr, "Incorrect dimension value: it should be positive integer\n");
            break; 
        }
        case INCORRECT_TEST_NUMBER_VALUE:
        {
            fprintf(stderr, "Incorrect test value: it should be positive integer\n");
            break; 
        }
        default:
        {
            break;
        }
    }


}