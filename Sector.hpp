#ifndef SECTOR_HPP
#define SECTOR_HPP

#include <vector>
#include "Math.hpp"

using namespace std;

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

#endif  // SECTOR_HPP