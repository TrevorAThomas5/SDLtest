#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char** argv) {

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL failed to init " << SDL_GetError() << endl;
    }

    cout << "test";

    system("pause");

    return EXIT_SUCCESS;
}