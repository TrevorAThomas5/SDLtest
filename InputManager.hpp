#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL2/SDL.h>
#include "Player.hpp"
#include <vector>
#include "Bullet.hpp"

using namespace std;

/**
 * A sector of the map
 */
class InputManager {
  private:
    Player* player;
    vector<Bullet*>* bullets;
    bool keys[322];  // key press

  public:
    InputManager(Player* player, vector<Bullet*>* bullets) : player(player), bullets(bullets) {
        for(unsigned int i = 0; i < 322; i++) {
            keys[i] = false;
        }
    }
    
    SDL_bool input();
};

#endif  // INPUT_MANAGER_HPP