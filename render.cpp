#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <fstream>

#define WIDTH 608
#define HEIGHT 480

using namespace std;


/**
 * 2D data
 */
struct xy {
    float x, y;
};


/**
 * 3D data
 */
struct xyz {
    float x, y, z;
};


/**
 * a 2x2 matrix
 */
struct mat2x2 {
    float m11, m12;
    float m21, m22;
};


/**
 * A sector of the map
 */
class Sector {
  public:
    Sector() : npoints(0) {}

    float floor;
    float ceil;
    vector<xy> vertices;
    char* neighbors;
    unsigned int npoints;
};


/**
 * A player character
 */
class Player {
  public:
    Player() {}

    xyz position;
    xyz velocity;
    float angle;            // angle from 0 degrees to 360 degrees
    unsigned int sector;
};

// list of sectors in the map
static vector<Sector*> sectors;

// the player character
static Player player;

// key press
static bool keys[322];

// debug file
static ofstream errorFile;


/**
 * multiplies a 2d vector by a 2x2 matrix
 */
static xy matrixMultiplication(mat2x2 mat, xy vec) {


    return vec;
}


/**
 * load into data structure from map file
 */
static void LoadData() {
    // open the map data file
    FILE* fp = fopen("map.txt", "r");

    // vector of vertices
    vector<xy> vertices;

    // working character
    char c = NULL;

    // working string
    string s = "";

    // working sector pointer
    Sector* sector = NULL;

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
                // create a new sector on the heap
                sector = new Sector();

                s = "";

                // ' '
                fgetc(fp);

                // floor
                while((c = fgetc(fp)) != ' ') {
                    s += c;
                }
                sector->floor = stof(s);

                // ceil
                s = "";
                while((c = fgetc(fp)) != ' ') {
                    s += c;
                }
                sector->ceil = stof(s);

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
                    sector->vertices.push_back(vertices.at(stof(s)));
                    sector->npoints++;
                }

                // add this sector to the sector list
                sectors.push_back(sector);

                //reset
                sector = NULL;
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
    // free sectors
    for(unsigned int i = sectors.size()-1; i >= 0; i--) {
        delete sectors.at(i);
    }
}


/**
 * render the screen
 */
static void DrawScreen(SDL_Renderer* renderer) { 
    // draw the player
    SDL_RenderDrawPoint(renderer, player.position.x + (WIDTH/2), player.position.y + (HEIGHT/2));

    // draw sectors
    for(unsigned int i = 0; i < sectors.size(); i++) {
        // because the vertices are in clockwise order
        for(unsigned int j = 0; j < sectors.at(i)->npoints-1; j++) {
            SDL_RenderDrawLine(renderer, sectors.at(i)->vertices.at(j).x+(WIDTH/2), 
                                        sectors.at(i)->vertices.at(j).y+(HEIGHT/2), 
                                        sectors.at(i)->vertices.at(j+1).x+(WIDTH/2), 
                                        sectors.at(i)->vertices.at(j+1).y+(HEIGHT/2));
        }
        SDL_RenderDrawLine(renderer, sectors.at(i)->vertices.at(sectors.at(i)->npoints-1).x
                                            +(WIDTH/2), 
                                        sectors.at(i)->vertices.at(sectors.at(i)->npoints-1).y
                                            +(HEIGHT/2), 
                                        sectors.at(i)->vertices.at(0).x+(WIDTH/2), 
                                        sectors.at(i)->vertices.at(0).y+(HEIGHT/2));
    }
}


/**
 * runs the program
 */
int main(int argc, char** argv) {
    // open the error file
    errorFile.open("error.txt");
    errorFile.clear();

    // load map data
    LoadData();

    errorFile << sectors.at(0)->npoints << endl;

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
                //SDL_RenderClear(renderer);
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                DrawScreen(renderer);
                SDL_RenderPresent(renderer);

                // get new input from keyboard
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                    
                    if(event.type == SDL_KEYDOWN) {
                        keys[event.key.keysym.sym] = true;
                    }
                    if(event.type == SDL_KEYUP) {
                        keys[event.key.keysym.sym] = false;
                    }
                }
                
                // process the input
                if(keys[SDLK_w]) {
                    player.position.y -= 1.0f;
                }
                if(keys[SDLK_s]) {
                    player.position.y += 1.0f;
                }
                if(keys[SDLK_a]) {
                    player.position.x -= 1.0f;
                }
                if(keys[SDLK_d]) {
                    player.position.x += 1.0f;
                }
                
            }
        }

        if(renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if(window) {
            SDL_DestroyWindow(window);
        }
    }

    // end program
    UnloadData();
    SDL_Quit();
    errorFile.close();
    return 0;
}