#ifndef PLAYER_HPP
#define PLAYER_HPP

/**
 * the player character
 */
class Player {
  public:
    Player() : position({0, 0}), angle(0), sector(0) {}
    xy position;
    float angle;
    unsigned int sector;
};

#endif  // PLAYER_HPP