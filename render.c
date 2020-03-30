#include <iostream>
#include <SDL2/SDL.h>

#define WIDTH 608
#define HEIGHT 480

/**
 * 
 */
static struct sector {
    float floor, ceil;
    struct xy {float x, y;} *vertex;
    signed char *neighbors;                 // each edge may have a corresponding neighboring 
                                            // sector
    unsigned int npoints;                   // how many vertices there are
} *sectors = NULL;
static unsigned int NumSectors = 0;

/**
 * the player
 */
static struct player {
    struct xyz {float x, y, z;} position, 
                                velocity;
    float angle, anglesin, anglecos, yaw;
    unsigned int sector;                        // the sector the player is currently in
} player;

/**
 * load into data structure from map file
 */
static void LoadData() {
    FILE* fp = fopen("map.txt", "rt");
    if(!fp) {
        perror("map.txt");
        exit(1);
    }
    char Buf[256], word[256], *ptr;
    struct xy* vert = NULL, v;
    int n, m, NumVertices = 0;
    while(fgets(Buf, sizeof Buf, fp)) {
        switch (sscanf(ptr = Buf, "%32s%n", word, &n) == 1 ? word[0] : '\0')
        {
            case 'v': // vertex
                for(sscanf(ptr += n, )) {}
                break;
        }
    }
    fclose(fp);
    free(vert);
}

/**
 * free memory
 */
static void UnloadData() {
    for(unsigned int i = 0; i < NumSectors; i++) {
        free(sectors[i].vertex);
    }
    for(unsigned int i = 0; i < NumSectors; i++) {
        free(sectors[i].neighbors);
    }
    free(sectors);
    sectors = NULL;
    NumSectors = 0;
}

/**
 * 
 */
static void DrawScreen(SDL_Renderer* renderer) {
    
    // draw the player
    SDL_RenderDrawPoint(renderer, player.position.x + (WIDTH/2), player.position.y + (HEIGHT/2));

    // draw sectors
    for(unsigned int i = 0; i < NumSectors; i++) {
        // because the vertices are in clockwise order
        for(unsigned int j = 0; j < sectors[i].npoints-1; j++) {
            SDL_RenderDrawLine(renderer, sectors[i].vertex[j].x+(WIDTH/2), 
                                        sectors[i].vertex[j].y+(HEIGHT/2), 
                                        sectors[i].vertex[j+1].x+(WIDTH/2), 
                                        sectors[i].vertex[j+1].y+(HEIGHT/2));
        }
    }
}

/**
 * runs the program
 */
int main(int argc, char** argv) {

    // if successfully create video
    if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        // if successfully create renderer
        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                DrawScreen(renderer);
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
             
                // SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
            }
        }

        if(renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if(window) {
            SDL_DestroyWindow(window);
        }
    }

    SDL_Quit();
    return 0;
}