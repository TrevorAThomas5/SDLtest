#include "Renderer.hpp"


/**
 * render the screen
 */
void Renderer::drawScreen() { 
    // set draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // draw dividing line
    SDL_RenderDrawLine(renderer, HALF_WIDTH, 0, HALF_WIDTH, HEIGHT);

    // draw the bullets to the screen
    drawBullets();

    // the player has turned
    updateRotationMatrix(rot, player->angle);
    
    // draw left side
    drawPlayerCentered();

    // draw right side
    drawWorldCentered();

    // end render
    SDL_RenderPresent(renderer);
}


/**
 * draw the bullets to the screen
 */
void Renderer::drawBullets() {
    // draw bullet
    for(unsigned int i = 0; i < bullets->size(); i++) {
        bullets->at(i)->position.x -= sinf(bullets->at(i)->direction);
        bullets->at(i)->position.y -= cosf(bullets->at(i)->direction);
        SDL_RenderDrawPoint(renderer, bullets->at(i)->position.x + RIGHT, 
                        bullets->at(i)->position.y + HALF_HEIGHT);

        // TODO use a stack to store the bullets, once they have existed for a certain amount of
        // time, delete them
    }
}


/**
 * renders the game with an unmoveable player 
 */
void Renderer::drawPlayerCentered() {
    // draw the player still
    SDL_Rect rect;
    rect.x = -3.5;
    rect.y = -3.5;
    rect.w = 7;
    rect.h = 7;
    rect.x += LEFT;
    rect.y += HALF_HEIGHT;
    SDL_RenderFillRect(renderer, &rect);

    // draw the player's direction
    xy out;
    out.x = 0;
    out.y = -20;
    SDL_RenderDrawLine(renderer, LEFT, HALF_HEIGHT, 
                                 out.x + LEFT, out.y + HALF_HEIGHT);

    // draw line perpendicular to view direction
    xy p1;
    p1.x = -5;
    p1.y = -20;
    xy p2;
    p2.x = 5;
    p2.y = -20;
    SDL_RenderDrawLine(renderer, p1.x + LEFT, 
                                 p1.y + HALF_HEIGHT, 
                                 p2.x + LEFT, 
                                 p2.y + HALF_HEIGHT);

    // draw the sectors
    for(unsigned int i = 0; i < sectors->size(); i++) {
        // because the vertices are in clockwise order
        for(unsigned int j = 0; j < sectors->at(i)->npoints; j++) {
            // coords of vertex in world space
            xy world1;
            world1.x = sectors->at(i)->vertices.at(j).x - player->position.x;
            world1.y = sectors->at(i)->vertices.at(j).y - player->position.y;
            
            xy world2;
            // if last edge
            if(j == (sectors->at(i)->npoints)-1) {
                world2.x = sectors->at(i)->vertices.at(0).x - player->position.x;
                world2.y = sectors->at(i)->vertices.at(0).y - player->position.y;
            }
            else {
                world2.x = sectors->at(i)->vertices.at(j+1).x - player->position.x;
                world2.y = sectors->at(i)->vertices.at(j+1).y - player->position.y;
            }

            // coords rotated around the player
            xy post1 = matrixMultiplication(rot, world1);
            xy post2 = matrixMultiplication(rot, world2);
            SDL_RenderDrawLine(renderer, post1.x+LEFT, 
                                         post1.y+HALF_HEIGHT, 
                                         post2.x+LEFT, 
                                         post2.y+HALF_HEIGHT); 
        }                                
    }
}


/**
 * renders the game with an unmoveable world
 */
void Renderer::drawWorldCentered() {
    // draw the player
    SDL_Rect rect;
    rect.x = -3.5;
    rect.y = -3.5;
    rect.w = 7;
    rect.h = 7;
    rect.x += player->position.x + RIGHT;
    rect.y += player->position.y + HALF_HEIGHT;
    SDL_RenderFillRect(renderer, &rect);

    // draw the player's direction
    xy out;
    out.x = 0;
    out.y = -20;
    xy dir = matrixMultiplication(rot, out);
    SDL_RenderDrawLine(renderer, player->position.x + RIGHT, player->position.y + HALF_HEIGHT, 
                                 player->position.x - dir.x + RIGHT, player->position.y 
                                 + dir.y + HALF_HEIGHT);

    // draw line perpendicular to view direction
    xy p1;
    p1.x = -5;
    p1.y = -20;
    xy p2;
    p2.x = 5;
    p2.y = -20;
    xy perp1 = matrixMultiplication(rot, p1);
    xy perp2 = matrixMultiplication(rot, p2);
    SDL_RenderDrawLine(renderer, player->position.x - perp1.x + RIGHT, 
                                 player->position.y + perp1.y + HALF_HEIGHT, 
                                 player->position.x - perp2.x + RIGHT, 
                                 player->position.y + perp2.y + HALF_HEIGHT);
                                 
    // draw sectors
    for(unsigned int i = 0; i < sectors->size(); i++) {
        // because the vertices are in clockwise order
        for(unsigned int j = 0; j < sectors->at(i)->npoints-1; j++) {
            SDL_RenderDrawLine(renderer, sectors->at(i)->vertices.at(j).x+RIGHT, 
                                        sectors->at(i)->vertices.at(j).y+HALF_HEIGHT, 
                                        sectors->at(i)->vertices.at(j+1).x+RIGHT, 
                                        sectors->at(i)->vertices.at(j+1).y+HALF_HEIGHT);
        }
        SDL_RenderDrawLine(renderer, sectors->at(i)->vertices.at(sectors->at(i)->npoints-1).x
                                            +RIGHT, 
                                        sectors->at(i)->vertices.at(sectors->at(i)->npoints-1).y
                                            +HALF_HEIGHT, 
                                        sectors->at(i)->vertices.at(0).x+RIGHT, 
                                        sectors->at(i)->vertices.at(0).y+HALF_HEIGHT);
    }
}


/**
 * Clear the screen with black
 */
void Renderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}