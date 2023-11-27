#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char** argv)
{
    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL could not initialize!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }
    
    window = SDL_CreateWindow(
        "Pokemon Showdown Player Port",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1920,
        1080,
        0
    );

    if (window == NULL)
    {
        std::cerr << "Window could not be created!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, 0);

    SDL_Event e;
    bool windowShouldRun = true;

    while (windowShouldRun)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                windowShouldRun = false;
            }
        }
        if (!windowShouldRun)
        {
            continue;
        }

        SDL_SetRenderDrawColor(ren, 100, 100, 0, 255);
        SDL_Rect fullscreen = {0, 0, 1920, 1080};
        SDL_RenderFillRect(ren, &fullscreen);
        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
