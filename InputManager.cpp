#include "InputManager.hpp"
#define SPEED 0.5f

/**
 * handles user input
 */
SDL_bool InputManager::input() {
    // get new input from keyboard
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return(SDL_TRUE);
        }
        if(event.type == SDL_KEYDOWN) {
            /*
            if(event.key.keysym.sym == SDLK_r) {
                // shoot
                Bullet* bul = new Bullet();
                bul->position = player->position;
                bul->direction = player->angle;
                bullets->push_back(bul);
            }
            */
            //keys[event.key.keysym.sym] = true;
        }
        if(event.type == SDL_KEYUP) {
            //keys[event.key.keysym.sym] = false;
        }
    }

    /*
    // movement
    if(keys[SDLK_w]) {
        player->position.x -= SPEED * sinf(player->angle);
        player->position.y -= SPEED * cosf(player->angle);
    }
    if(keys[SDLK_s]) {
        player->position.x += SPEED * sinf(player->angle);
        player->position.y += cosf(player->angle);
    }
    if(keys[SDLK_a]) {
        player->position.x -= SPEED * cosf(player->angle);
        player->position.y += SPEED * sinf(player->angle);
    }
    if(keys[SDLK_d]) {
        player->position.x += SPEED * cosf(player->angle);
        player->position.y -= SPEED * sinf(player->angle);
    }

    // rotation
    if(keys[SDLK_q]) {
        player->angle += 0.02f;
    }
    if(keys[SDLK_e]) {
        player->angle -= 0.02f;
    }   
    */

    return SDL_FALSE;             
}
