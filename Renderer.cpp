#include "Renderer.hpp"

#include <iostream>
#include <time.h>


/**
 * constructs a new renderer
 */
Renderer::Renderer(SDL_Renderer* renderer, Player* player, vector<Sector*>* sectors, 
             vector<Bullet*>* bullets, ofstream& errorFile) : renderer(renderer), sectors(sectors), 
             bullets(bullets), player(player), errorFile(errorFile) {
               rot = new mat2x2;
               projMat = createProjectionMatrix();
            
}

/**
 * frees memory
 */
Renderer::~Renderer() {
    delete rot;
    delete projMat;
}


/**
 * render the screen
 */
void Renderer::drawScreen() { 
    // set draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // draw dividing line
    SDL_RenderDrawLine(renderer, .333f*WIDTH, 0, .333f*WIDTH, HEIGHT);
    SDL_RenderDrawLine(renderer, .666f*WIDTH, 0, .666f*WIDTH, HEIGHT);
    
    // spin player/camera around the object 
    player->position.x = 200 * sinf((float)SDL_GetTicks() / 600);
    player->position.y = 200 * cosf((float)SDL_GetTicks() / 600);

    // change player look angle to look at the center
    player->angle = (M_PI / 2) - atan2(player->position.y, player->position.x);

    // the player has turned
    updateRotationMatrix(rot, player->angle);
   
    // draw the bullets to the screen
    drawBullets();
    
    // draw 3D
    draw3D();

    // draw left side
    drawPlayerCentered();

    // draw right side
    drawWorldCentered();

    // end render
    SDL_RenderPresent(renderer);
}

/**
 * render 3D scene
 */
void Renderer::draw3D() {
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

            xyzw view1low;
            view1low.x = post1.x;
            view1low.y = sectors->at(i)->floor;
            view1low.z = post1.y;
            view1low.w = 1.0f;
            xyzw view1high;
            view1high.x = post1.x;
            view1high.y = sectors->at(i)->ceil; 
            view1high.z = post1.y;
            view1high.w = 1.0f;

            xyzw view2low;
            view2low.x = post2.x;
            view2low.y = sectors->at(i)->floor;
            view2low.z = post2.y;
            view2low.w = 1.0f;
            xyzw view2high;
            view2high.x = post2.x;
            view2high.y = sectors->at(i)->ceil; 
            view2high.z = post2.y;
            view2high.w = 1.0f;

            xy screen1low;
            screen1low.x = (NEAR_PLANE * view1low.x / -view1low.z) * HALF_WIDTH/2;
            screen1low.y = -(NEAR_PLANE * view1low.y / -view1low.z) * HALF_HEIGHT;
            xy screen1high;
            screen1high.x = (NEAR_PLANE * view1high.x / -view1high.z) * HALF_WIDTH/2;
            screen1high.y = -(NEAR_PLANE * view1high.y / -view1high.z) * HALF_HEIGHT;
            xy screen2low;
            screen2low.x = (NEAR_PLANE * view2low.x / -view2low.z) * HALF_WIDTH/2;
            screen2low.y = -(NEAR_PLANE * view2low.y / -view2low.z) * HALF_HEIGHT;
            xy screen2high;
            screen2high.x = (NEAR_PLANE * view2high.x / -view2high.z) * HALF_WIDTH/2;
            screen2high.y = -(NEAR_PLANE * view2high.y / -view2high.z) * HALF_HEIGHT;

            SDL_RenderDrawLine(renderer, screen1low.x+HALF_WIDTH, screen1low.y+HALF_HEIGHT, 
                               screen1high.x+HALF_WIDTH, screen1high.y+HALF_HEIGHT);
            SDL_RenderDrawLine(renderer, screen1low.x+HALF_WIDTH, screen1low.y+HALF_HEIGHT, 
                               screen2low.x+HALF_WIDTH, screen2low.y+HALF_HEIGHT);
            SDL_RenderDrawLine(renderer, screen2low.x+HALF_WIDTH, screen2low.y+HALF_HEIGHT, 
                               screen2high.x+HALF_WIDTH, screen2high.y+HALF_HEIGHT);
            SDL_RenderDrawLine(renderer, screen1high.x+HALF_WIDTH, screen1high.y+HALF_HEIGHT, 
                               screen2high.x+HALF_WIDTH, screen2high.y+HALF_HEIGHT);
                               
        }                                
    }
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
    rect.x += .1666f*WIDTH;
    rect.y += HALF_HEIGHT;
    SDL_RenderFillRect(renderer, &rect);

    // draw the player's direction
    xy out;
    out.x = 0;
    out.y = -20;
    SDL_RenderDrawLine(renderer, .1666f*WIDTH, HALF_HEIGHT, 
                                 out.x + .1666f*WIDTH, out.y + HALF_HEIGHT);

    // draw line perpendicular to view direction
    xy p1;
    p1.x = -5;
    p1.y = -20;
    xy p2;
    p2.x = 5;
    p2.y = -20;
    SDL_RenderDrawLine(renderer, p1.x + .1666f*WIDTH, 
                                 p1.y + HALF_HEIGHT, 
                                 p2.x + .1666f*WIDTH, 
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
            SDL_RenderDrawLine(renderer, post1.x+.1666f*WIDTH, 
                                         post1.y+HALF_HEIGHT, 
                                         post2.x+.1666f*WIDTH, 
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
    rect.x += player->position.x + .8333*WIDTH;
    rect.y += player->position.y + HALF_HEIGHT;
    SDL_RenderFillRect(renderer, &rect);

    // draw the player's direction
    xy out;
    out.x = 0;
    out.y = -20;
    xy dir = matrixMultiplication(rot, out);
    SDL_RenderDrawLine(renderer, player->position.x + .8333*WIDTH, player->position.y + HALF_HEIGHT, 
                                 player->position.x - dir.x + .8333*WIDTH, player->position.y 
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
    SDL_RenderDrawLine(renderer, player->position.x - perp1.x + .8333*WIDTH, 
                                 player->position.y + perp1.y + HALF_HEIGHT, 
                                 player->position.x - perp2.x + .8333*WIDTH, 
                                 player->position.y + perp2.y + HALF_HEIGHT);
                                 
    // draw sectors
    for(unsigned int i = 0; i < sectors->size(); i++) {
        // because the vertices are in clockwise order
        for(unsigned int j = 0; j < sectors->at(i)->npoints-1; j++) {
            SDL_RenderDrawLine(renderer, sectors->at(i)->vertices.at(j).x+.8333*WIDTH, 
                                        sectors->at(i)->vertices.at(j).y+HALF_HEIGHT, 
                                        sectors->at(i)->vertices.at(j+1).x+.8333*WIDTH, 
                                        sectors->at(i)->vertices.at(j+1).y+HALF_HEIGHT);
        }
        SDL_RenderDrawLine(renderer, sectors->at(i)->vertices.at(sectors->at(i)->npoints-1).x
                                            +.8333*WIDTH, 
                                        sectors->at(i)->vertices.at(sectors->at(i)->npoints-1).y
                                            +HALF_HEIGHT, 
                                        sectors->at(i)->vertices.at(0).x+.8333*WIDTH, 
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


/**
 * creates projection matrix
 */
mat4x4* Renderer::createProjectionMatrix() {
    // precalculations
    float aspectRatio = WIDTH / HEIGHT;
    float y_scale = ((1.0f / tanf( ((FOV / 2.0f) * 3.141592f)/180.0f )) * aspectRatio);
    float x_scale = y_scale / aspectRatio;
    float frustum_length = FAR_PLANE - NEAR_PLANE;

    // fills the matrix
    mat4x4* projectionMatrix = new mat4x4();
    projectionMatrix->m00 = x_scale;
    projectionMatrix->m11 = y_scale;
    projectionMatrix->m22 = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
    projectionMatrix->m23 = -1.0f;
    projectionMatrix->m32 = -((2.0f * NEAR_PLANE * FAR_PLANE) / frustum_length);
    projectionMatrix->m33 = 0;

    return projectionMatrix;
}

/**
 * creates view matrix
 *
mat4x4* Renderer::createViewMatrix() {
    mat4x4* ret = new mat4x4();

    xyz front = player.angle;
    xyz up; 
    up.x=0;
    up.y=1.0f;
    up.z=0;
    xyz side;

    ret->m00 = side.x;
    ret->m10 = side.y;
    ret->m20 = side.z;
    ret->m01 = up.x;
    ret->m11 = up.y;
    ret->m21 = up.z;
    ret->m02 = -front.x;
    ret->

    return ret;
}
*/
