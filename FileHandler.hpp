#ifndef FILE_HANDLER_HPP
#define FILE_HAND_HPP

#include <fstream>
#include <string>

#include "Sector.hpp"


/**
 * handles file data
 */
class FileHandler {
  private:
    FILE* fp;
    vector<Sector*>* sectors;

  public:
    FileHandler(char* fileName, vector<Sector*>* sects);
    void loadData();
    void unloadData();

};

#endif  // FILE_HANDLER_HPP