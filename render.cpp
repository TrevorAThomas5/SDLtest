#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <string>

#define WIDTH 608
#define HEIGHT 480

using namespace std;

struct xy {
    float x, y;
};

struct xyz {
    float x, y, z;
};

static class sector {
  public:
    float floor;
    float ceil;
    vector<xy> vertices;
    char* neighbors;
    unsigned int npoints;
}* sectors;
static unsigned int NumSectors = 0;

static class player {
  public:
    xyz position;
    xyz velocity;
    float angle, anglesin, anglecos, yaw;
    unsigned int sector;
} player;

/**
 * load into data structure from map file
 */
static void LoadData() {
    vector<xy> vertices;
    vector<sector> sectorList;
    FILE* fp = fopen("map.txt", "r");
    char c = NULL;
    string s = "";

    // process file
    while(c != EOF) {
        switch (c = fgetc(fp))
        {
            case 'v':
                s = "";
                xy vert;

                // ' '
                fgetc(fp);

                // x
                while((c = fgetc(fp)) != ' ') {
                    s += c;
                }
                vert.x = stof(s);

                // y
                s = "";
                while((c = fgetc(fp)) != '\n') {
                    s += c;
                }
                vert.y = stof(s);

                // add vertex to the vector
                vertices.push_back(vert);
                break;
            case 's':
                sector sector;
                s = "";

                // ' '
                fgetc(fp);

                // floor
                while((c = fgetc(fp)) != ' ') {
                    s += c;
                }
                sector.floor = stof(s);

                // ceil
                s = "";
                while((c = fgetc(fp)) != ' ') {
                    s += c;
                }
                sector.ceil = stof(s);

                // vertices
                while(c != '\n') {
                    s = "";
                    while((c = fgetc(fp)) != ' ') {
                        if(c == '\n') {
                            break;
                        }
                        s += c;
                    }
                    if(s == "") {
                        break;
                    }
                    sector.vertices.push_back(vertices.at(stof(s)));
                }

                sectors.push_back(sector);

                break;
            default:
                break;
        }
    }

    fclose(fp);
}

/**
 * free memory
 */
static void UnloadData() {
    for(unsigned int i = 0; i < NumSectors; i++) {
        free(sectors.at(i).vertices);
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
            SDL_RenderDrawLine(renderer, sectors[i].vertices[j].x+(WIDTH/2), 
                                        sectors[i].vertices[j].y+(HEIGHT/2), 
                                        sectors[i].vertices[j+1].x+(WIDTH/2), 
                                        sectors[i].vertices[j+1].y+(HEIGHT/2));
        }
    }
}

/**
 * runs the program
 */
int main(int argc, char** argv) {

    LoadData();

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

    UnloadData();

    SDL_Quit();
    return 0;
}