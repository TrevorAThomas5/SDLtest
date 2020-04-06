#include "FileHandler.cpp"
#include "Player.hpp"
#include "Renderer.cpp"
#include "InputManager.cpp"


/**
 * runs the program
 */
int main(int argc, char** argv) {
    // open the error file
    ofstream errorFile;
    errorFile.open("error.txt");
    errorFile.clear();

    // load map data
    vector<Sector*> sectors;
    FileHandler* fileHandler = new FileHandler("map.txt", &sectors);
    fileHandler->loadData();
    Player* player = new Player();
    vector<Bullet*> bullets;
    InputManager* inputManager = new InputManager(player, &bullets);

    // if successfully create video
    if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = nullptr;
        SDL_Renderer* rendererSDL = nullptr;

        // if successfully create renderer
        if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &rendererSDL) == 0) {
            SDL_bool done = SDL_FALSE;
            Renderer* renderer = new Renderer(rendererSDL, player, &sectors, &bullets, errorFile);
           
            while (!done) {
                renderer->clearScreen();
                renderer->drawScreen();
                done = inputManager->input();
            }
            delete renderer;
        }

        // clear SDL memory
        if(rendererSDL) {
            SDL_DestroyRenderer(rendererSDL);
        }
        if(window) {
            SDL_DestroyWindow(window);
        }
    }

    // end program
    fileHandler->unloadData();
    SDL_Quit();
    errorFile.close();
    delete fileHandler;
    delete player;
    delete inputManager;
    return 0;
}