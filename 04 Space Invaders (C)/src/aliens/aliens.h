#ifndef ALIENS_H
#define ALIENS_H

#define ALIENS_NUM_ALIEN_STANCES 4 //Number of stances an alien can have
#define ALIENS_NUM_ALIENS_WIDE 11 //Number of columns of aliens
#define ALIENS_NUM_ALIENS_HIGH 5 //Number of rows of aliens

#include <stdint.h>
#include <stdbool.h>

enum alien_stances
{
    alien_out, //alien legs are out
    alien_in, //alien legs are in
    alien_explode, //Alien is exploding
    alien_gone //Alien is dead
};

struct Alien
{
    uint32_t x; //x position of the alien
    uint32_t y; //y position of the alien
    const uint32_t *alien[ALIENS_NUM_ALIEN_STANCES]; //Array of the various sprites representing the various stances
    enum alien_stances stance; //Current stance of the alien
    bool isWrittenGone; //If the alien is currently gone
};

struct Point //Represents the current point of a certain alien shooter
{
    uint32_t x;
    uint32_t y;
};

void aliens_init();

void aliens_tick();

void aliens_enable();

void aliens_disable();

bool aliens_checkIfAllAliensAreHit();

void aliens_hit(uint32_t x, uint32_t y);

struct Point aliens_getBottomMiddleOfAlien(uint32_t i);

#endif // ALIENS_H
