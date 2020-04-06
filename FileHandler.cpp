#include "FileHandler.hpp"

/**
 * construct a new fileHandler
 */
FileHandler::FileHandler(char* fileName, vector<Sector*>* sects) {
    fp = fopen(fileName, "r");
    sectors = sects;
}

/**
 * load into data structure from map file
 */
void FileHandler::loadData() {
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
                sectors->push_back(sector);

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
void FileHandler::unloadData() {
    // free sectors
    for(unsigned int i = sectors->size()-1; i >= 0; i--) {
        delete sectors->at(i);
    }
}