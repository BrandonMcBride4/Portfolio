#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>

#define GLOBALS_STARTING_NUM_LIVES 3 // Starting number of lives
#define GLOBALS_GAME_OVER_LIVES 0 // Number of lives that indicates a game over
#define GLOBALS_STARTING_SCORE 0     // Starting score

// Set the button value
void globals_setBtnValue(uint32_t value);

// Get the button value
uint32_t globals_getBtnValue();

// Set the switch value
void globals_setSwValue(uint32_t value);

// Get the switch value
uint32_t globals_getSwValue();

// Set the number of lives
void globals_setNumLives(uint32_t value);

// increment lives by one
void globals_incLives();

// decrement lives by one
void globals_decLives();

// Get the current number of lives
uint32_t globals_getNumLives();

// Set the score
void globals_setScore(uint32_t value);

// Get the current number of lives
uint32_t globals_getScore();

// it increment the score
void globals_incScore(uint32_t incValue);


#endif // GLOBAL_H
