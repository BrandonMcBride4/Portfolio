#include "globals.h"
#include "../image_render/image_render.h"

#define MAX_NUM_LIVES 5 //Maximum number of lives permitted
#define MIN_NUM_LIVES 0 //Minimum number of lives permitted
#define MAX_SCORE 99999 //Maximum score permitted
#define MIN_SCORE 0 //Minimum score permitted

static uint32_t btnValue = 0; //represent the value of the button
static uint32_t swValue = 0; //Represents the value of the switches
static uint32_t numLives = GLOBALS_STARTING_NUM_LIVES; //Represents the current number of lies
static uint32_t score = 0; //Represents the current score


// Set the button value
void globals_setBtnValue(uint32_t value)
{ btnValue = value; }

// Get the button value
uint32_t globals_getBtnValue()
{ return btnValue; }

// Set the switch value
void globals_setSwValue(uint32_t value)
{ swValue = value; }

// Get the switch value
uint32_t globals_getSwValue()
{ return swValue; }

// Set the number of lives
void globals_setNumLives(uint32_t value)
{
    if (value > MAX_NUM_LIVES) //Don't set the lives to more than the maximum
    {
        value = MAX_NUM_LIVES;
    }
    else if (value < MIN_NUM_LIVES) //Don't set the lives to less than the minimum
    {
        value = MIN_NUM_LIVES;
    }

    numLives = value;
    image_render_lives(numLives); //Render the lives onto the screen
}

// Increments lives by one
void globals_incLives()
{
    globals_setNumLives(numLives + 1);
}

// Decrements lives by one
void globals_decLives()
{
    globals_setNumLives(numLives - 1);
}

// Get the current number of lives
uint32_t globals_getNumLives()
{ return numLives; }

// Set the switch value
void globals_setScore(uint32_t value)
{
    if (value > MAX_SCORE)  //Don't set the score to more than the maximum
    {
        value = MAX_SCORE;
    }
    if (value < MIN_SCORE) //Don't set the score to less than the minimum
    {
        value = MIN_SCORE;
    }

    score = value;
}

// Get the switch value
uint32_t globals_getScore()
{ return score; }

//Increment the score
void globals_incScore(uint32_t incValue)
{
    globals_setScore(score + incValue);
}
