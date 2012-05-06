#include <stdio.h>
#include <SDL/SDL.h>

void main(void)
{
    int result;
    result = SDL_Init(SDL_INIT_EVERYTHING);
    if(result == 0)
    {
        printf("SDL INSTALLED SUCCESSFUL\n");
    }
    else
    {
         printf("SDL INSTALLED FAILED\n");
    }
}
