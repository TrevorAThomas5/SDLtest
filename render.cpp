#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char** argv) {

    // if successfully create video
    if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        // if successfully create renderer
        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
            SDL_Event event;
        }
    }


    return 0;
}