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
    bool keys[322];  // key press
    Player* player;
    vector<Bullet*>* bullets;

  public:
    InputManager(Player* player, vector<Bullet*>* bullets) : player(player), bullets(bullets) {}
    SDL_bool input();
};

#endif  // INPUT_MANAGER_HPP