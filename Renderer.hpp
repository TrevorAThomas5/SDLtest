#ifndef RENDERER_HPP
#define RENDERER_HPP

#define HEIGHT 480
#define WIDTH 1000
#define FOV 90
#define HALF_HEIGHT 240
#define HALF_WIDTH 500
#define LEFT 250
#define RIGHT 750

#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <math.h>
#include <vector>
#include "Player.hpp"
#include "Bullet.hpp"
#include "Sector.hpp"

using namespace std;

/**
 * renders the screen
 */
class Renderer {
  private:
    SDL_Renderer* renderer;
    mat2x2* rot;       // rotation matix
    Player* player;   // pointer to the player
    vector<Bullet*>* bullets;
    vector<Sector*>* sectors;


  public:
    Renderer(SDL_Renderer* renderer, Player* player, vector<Sector*>* sectors, 
             vector<Bullet*>* bullets) : renderer(renderer), sectors(sectors), 
             bullets(bullets), player(player) {
               rot = new mat2x2;
             }
    void drawScreen();
    void drawBullets();
    void drawPlayerCentered();
    void drawWorldCentered();
    void clearScreen();

};

#endif  // RENDERER_HPP