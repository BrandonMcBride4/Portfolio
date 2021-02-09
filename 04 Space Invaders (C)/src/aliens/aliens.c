#include "aliens.h"
#include <stdio.h>
#include <stdlib.h>
#include "../image_render/image_render.h"
#include "../image_render/sprite.h"
#include "../video/video.h"
#include "../globals/globals.h"
#include "../tank_bullet/tank_bullet.h"
#include "../user_audio/user_audio.h"


#define ALIEN_ROW_1 0 //Represents the first row of aliens
#define ALIEN_ROW_2 1 //Represents the second row of aliens
#define ALIEN_ROW_3 2 //Represents the third row of aliens
#define ALIEN_ROW_4 3 //Represents the fourth row of aliens
#define ALIEN_ROW_5 4 //Represents the fifth row of aliens

#define ALIENS_LEFT_LIMIT 20 //The furthest left position that the aliens travel in pixels
#define ALIENS_RIGHT_LIMIT VIDEO_SCREEN_WIDTH - IMAGE_RENDER_ALIEN_ARMY_PIXEL_WIDTH - ALIENS_LEFT_LIMIT //The furthest right position that the aliens travel in pixels
#define ALIENS_Y_START 70 //The y coordinate of the aliens relative to the top of the screen
#define ALIENS_MARCH_DOWN_DISTANCE 10 //Distance that the aliens go down when they reach the end of the screen
#define ALIENS_TICKS_PER_INCREMENT 25 //The number of ticks that pass before the aliens moves another increment
#define TICKS_PER_SECOND 100 //Number of clock ticks that compose one second
#define RESET 0 //Used to reset counters
#define TOTAL_ALIEN_COUNT ALIENS_NUM_ALIENS_WIDE * ALIENS_NUM_ALIENS_HIGH //The total number of aliens that are originally rendered

#define TOP_ROW_POINTS 40 // Number of points that are awarded for the top aliens
#define MIDDLE_ROW_POINTS 20 // Number of points that are awarded for the middle aliens
#define BOTTOM_ROW_POINTS 10 // Number of points that are awarded for the bottom aliens

#define ALL_GONE_COORDINATE 0 //Coordinate indicating the aliens in a certain column are all gone
#define HALF 0.5 //Macro for splitting a value in half

#define NO_COLUMN_RETURN_VALUE 0 // Value returned when no aliens are left

static int32_t aliens_x_pos = ALIENS_LEFT_LIMIT; //Keeps track of the current aliens x position
static int32_t aliens_y_pos = ALIENS_Y_START; // Keeps track of the current aliens y position
static uint32_t aliens_ticks_since_increment = 0; //Keeps track of the number of clock ticks that have passsed since the last time the aliens incremented a pixel
static bool isEnabled; //Boolean indcating that the aline ns state machine is enabled
static uint32_t numAliensHit; //Number of aliens that have been git
static uint32_t marchSoundCounter = 0; // Used to choose which march sound to play

static struct Alien alienArmy[ALIENS_NUM_ALIENS_WIDE][ALIENS_NUM_ALIENS_HIGH]; //2D array representing all of the aliens


static enum aliens_st_t
{
    init_st, //Starting state
    move_left_st, //Moves aliens left across the screen
    move_right_st //Moves aliens right across the screen
} currentState;

void initAlienArmy() //Initialize the army of aliens
{
    for (uint32_t i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i) //Iterate through the columns of aliens
    {
        for (uint32_t j = 0; j < ALIENS_NUM_ALIENS_HIGH; ++j) //Iterate through the rows of aliens
        {
            switch (j) //Determine which row we are on
            {
                case ALIEN_ROW_1: //Get alien sprites for first row
                    alienArmy[i][j].alien[alien_out] = alien_top_out_14x10;
                    alienArmy[i][j].alien[alien_in] = alien_top_in_14x10;
                    break;
                case ALIEN_ROW_2: //Get alien sprites for second row
                    alienArmy[i][j].alien[alien_out] = alien_middle_out_14x10;
                    alienArmy[i][j].alien[alien_in] = alien_middle_in_14x10;
                    break;
                case ALIEN_ROW_3: //Get alien sprites for third row
                    alienArmy[i][j].alien[alien_out] = alien_middle_out_14x10;
                    alienArmy[i][j].alien[alien_in] = alien_middle_in_14x10;
                    break;
                case ALIEN_ROW_4: //Get alien sprites for fourth row
                    alienArmy[i][j].alien[alien_out] = alien_bottom_out_14x10;
                    alienArmy[i][j].alien[alien_in] = alien_bottom_in_14x10;
                    break;
                case ALIEN_ROW_5: //Get alien sprites for fifth row
                    alienArmy[i][j].alien[alien_out] = alien_bottom_out_14x10;
                    alienArmy[i][j].alien[alien_in] = alien_bottom_in_14x10;
                    break;
            }

            alienArmy[i][j].alien[alien_explode] = alien_explosion_14x10; //Initialize the alien explosion sprites
            alienArmy[i][j].alien[alien_gone] = alien_gone_14x10; //Initialize the erase alien sprites
            alienArmy[i][j].stance = alien_out; //Initialize the current stance of the alien

            alienArmy[i][j].isWrittenGone = false; //Initialize the alien to not being gone
        }
    }
}

int leftMostAlienColumn() //Returns the left most column with aliens
{
    for (uint32_t i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i) //Iterate through columns
    {
        for (uint32_t j = 0; j < ALIENS_NUM_ALIENS_HIGH; ++j) //Iterate through rows
        {
            if (alienArmy[i][j].stance != alien_gone) //If there is an alien in this column return the column number
            {
                return i;
            }
        }
    }
    return NO_COLUMN_RETURN_VALUE;
}

int rightMostAlienColumn() //Returns the right most column with aliens
{
    for (int32_t i = ALIENS_NUM_ALIENS_WIDE - 1; i >= 0; --i) //Iterate through columns from right
    {
        for (uint32_t j = 0; j < ALIENS_NUM_ALIENS_HIGH; ++j) //Iterate through rows
        {
            if (alienArmy[i][j].stance != alien_gone) //If there is an alien in this column return the column number
            {
                return i;
            }
        }
    }
    return NO_COLUMN_RETURN_VALUE;
}

void aliens_init() //Initialize the aliens
{
    aliens_enable(); //Enable the alien state machine
    initAlienArmy(); //Initialize the alien army
    currentState = init_st; // Initialize the state machine
    aliens_x_pos = ALIENS_LEFT_LIMIT; //Keeps track of the current aliens x position
    aliens_y_pos = ALIENS_Y_START; // Keeps track of the current aliens y position
    image_render_alienArmy(aliens_x_pos, aliens_y_pos, alienArmy); //Draw the army of aliens
    aliens_ticks_since_increment = RESET; //Reset the ticks since increment counter
    marchSoundCounter = RESET; // Reset the sound counter for the music
}

// Changes the alien state from in to out or vice versa
void alienMarch()
{
    // If some other sound is playing reset counter
    if (user_audio_isPlaying())
    {
        marchSoundCounter = RESET;
    }
    else
    {
        user_audio_playMarch(marchSoundCounter); // Play background music
        marchSoundCounter++; // Changes walk sound
    }

    for (uint32_t i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i) //Iterate through the columns
    {
        for (uint32_t j = 0; j < ALIENS_NUM_ALIENS_HIGH; ++j) //Iterate through the rows
        {
            // If it's in change to out
            if (alienArmy[i][j].stance == alien_in)
            {
                alienArmy[i][j].stance = alien_out;
            }
                // If it's out change to in
            else if (alienArmy[i][j].stance == alien_out)
            {
                alienArmy[i][j].stance = alien_in;
            }
        }
    }
}

// Write a blank line at the top of each aliens
void eraseAliensTemporarily()
{
    int leftColumnIndex = leftMostAlienColumn(); //column of the left most alien
    int rightColumnIndex = rightMostAlienColumn(); //column of the right most alien
    int lineWidth = (rightColumnIndex - leftColumnIndex + 1) * IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH; //Width of the line to erase aliens
    for (uint32_t i = 0; i < ALIENS_NUM_ALIENS_HIGH; ++i) //Iterate through rows
    {
        for (int j = 0; j < ALIENS_MARCH_DOWN_DISTANCE; ++j) //Distnace that alience marhc down
        {
            video_writeHLine(aliens_x_pos + (leftColumnIndex * IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH), aliens_y_pos + j + i*IMAGE_RENDER_ALIEN_ARMY_BLOCK_HEIGHT, IMAGE_RENDER_BACKGROUND_COLOR, lineWidth); //Write the erase line
        }
    }
}

void moveAliensDown()
{
    eraseAliensTemporarily();
    aliens_y_pos += ALIENS_MARCH_DOWN_DISTANCE; // Move aliens down
    image_render_alienArmy(aliens_x_pos, aliens_y_pos, alienArmy); // Redraw aliens
    // If aliens reach bunkers, then game over
    uint32_t mostBottomAlienY = 0; //The y coordinate of the bottom most alien`
    uint32_t columnBottomAlienY = 0; //The most bottom alien of a certain column
    // Gets the y coordinate of the bottom most alien of the whole army
    for (int i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i) //Iterate through the aliens to get the bottom most alien
    {
        columnBottomAlienY = aliens_getBottomMiddleOfAlien(i).y; //Get the bottom middle alien of the column
        if (columnBottomAlienY > mostBottomAlienY) //If the bottom alien of the column is larger the current most bottom alien
        {
            mostBottomAlienY = columnBottomAlienY; //Set the most bottom alien to that value
        }
    }
    if (mostBottomAlienY >= IMAGE_RENDER_BUNKER_Y) //If the bottom most aliens have reached the bunker height
    {
        globals_setNumLives(GLOBALS_GAME_OVER_LIVES); //Set game over
    }
}

void aliens_enable() //enable aliens
{
    isEnabled = true;
}

void aliens_disable() //disable aliens
{
    isEnabled = false;
}

void aliens_tick() //Update alien state machine every tick
{
    //state update
    switch (currentState)
    {
        case init_st: //Starting state
            if (isEnabled)
            {
                currentState = move_right_st; //Begin moving right
            }
            else
            {
                currentState = init_st; // If not enabled go to init
            }
            break;
        case move_left_st: //Moves aliens left across the screen
            if (!isEnabled)
            {
                currentState = init_st; // If not enabled go to init
            }
            // If it's at the left limit, go down and start going right
            else if (aliens_x_pos + (leftMostAlienColumn() * IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH) < ALIENS_LEFT_LIMIT )
            {
                currentState = move_right_st; //Transition ot move right state
                alienMarch();   // Change the alien stance
                moveAliensDown(); //Move the aliens down a level
            }
            break;
        case move_right_st: //Moves aliens right across the screen
            if (!isEnabled)
            {
                currentState = init_st; // If not enabled go to init
            }
            // If it's at the right limit, go down and start going left
            else if (aliens_x_pos - ((ALIENS_NUM_ALIENS_WIDE - rightMostAlienColumn() - 1) * IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH) > ALIENS_RIGHT_LIMIT)
            {
                currentState = move_left_st; //transition to move left state
                alienMarch();   // Change the alien stance
                moveAliensDown(); //Move the aliens down a level
            }
            break;
        default:
            printf("aliens_tick state update hit default");
            break;
    }

    //state action
    switch (currentState)
    {
        case init_st: //Starting state
            break;
        case move_left_st: //Moves aliens left across the screen
            aliens_ticks_since_increment++;
            if (aliens_ticks_since_increment == ALIENS_TICKS_PER_INCREMENT)
            {
                alienMarch(); // Change the alien's stance
                image_render_alienArmy(--aliens_x_pos, aliens_y_pos, alienArmy); // Move to the left
                aliens_ticks_since_increment = RESET; // Reset counter
            }
            break;
        case move_right_st: //Moves aliens right across the screen
            aliens_ticks_since_increment++;
            if (aliens_ticks_since_increment == ALIENS_TICKS_PER_INCREMENT)
            {
                alienMarch(); // Transition the alien to a different stance
                image_render_alienArmy(++aliens_x_pos, aliens_y_pos, alienArmy); // Move to the right
                aliens_ticks_since_increment = RESET; // Reset counter
            }
            break;
        default:
            printf("aliens_tick state action hit default");
            break;
    }
}

bool aliens_checkIfAllAliensAreHit() //Check to see if all of the aliens have been hit.
{
    return numAliensHit >= TOTAL_ALIEN_COUNT; //If the number of aliens hit is the same as the total number of aliens it will return true
}

void aliens_hit(uint32_t x, uint32_t y) // Sequence to go through when an alien is hit
{
    user_audio_playInvaderDie(); // Play sound
    numAliensHit = RESET; //Reset the counter indicating the number of alines that are hit
    for (uint32_t i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i) // Iterate thrugh the columns
    {
        for (uint32_t j = 0; j < ALIENS_NUM_ALIENS_HIGH; ++j) //Iterate through the rows
        {
            //If the position of the bullet is at this alien location
            if (x >= alienArmy[i][j].x && x <= alienArmy[i][j].x + IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH &&
                y >= alienArmy[i][j].y && y <= alienArmy[i][j].y + IMAGE_RENDER_ALIEN_ARMY_BLOCK_HEIGHT)
            {
                alienArmy[i][j].stance = alien_explode; //Set the stance to the exploding stance
                switch (j) //Determine points earned based on alien row
                {
                    case ALIEN_ROW_1: //Set points for if the first row was hit
                        globals_incScore(TOP_ROW_POINTS);
                        image_render_updateScore();
                        break;
                    case ALIEN_ROW_2: //Set points for if the second row was hit
                        globals_incScore(MIDDLE_ROW_POINTS);
                        image_render_updateScore();
                        break;
                    case ALIEN_ROW_3: //Set points for if the third row was hit
                        globals_incScore(MIDDLE_ROW_POINTS);
                        image_render_updateScore();
                        break;
                    case ALIEN_ROW_4: //Set points for if the fourth row was hit
                        globals_incScore(BOTTOM_ROW_POINTS);
                        image_render_updateScore();
                        break;
                    case ALIEN_ROW_5: //Set points for if the fifth row was hit
                        globals_incScore(BOTTOM_ROW_POINTS);
                        image_render_updateScore();
                        break;
                }
            }
            if (alienArmy[i][j].isWrittenGone || alienArmy[i][j].stance == alien_explode) //If the alien is gone
            {
                numAliensHit++; //Increment the number of aliens that have been hit
            }
        }
    }

    if (aliens_checkIfAllAliensAreHit()) //if all of the aliens are gone, erase all alien
    {
        for (uint32_t i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i) //Iterate through the columns
        {
            for (uint32_t j = 0; j < ALIENS_NUM_ALIENS_HIGH; ++j) //Iterate through the rows
            {
                image_render_singleAlien(alienArmy[i][j].x, alienArmy[i][j].y, alien_gone_14x10); //Erase the alien
            }
        }
        tank_bullet_init(); //Reset the tank bullets
        aliens_init();// Reset the aliens
        globals_incLives(); //Increment the number of livers (level up!)
    }

}

// Returns x and y coordinates of where to start the alien bullet at a given column number
struct Point aliens_getBottomMiddleOfAlien(uint32_t i)
{
    struct Point point; //point at which to start the bullet
    point.x = ALL_GONE_COORDINATE; //Initialize x to zero
    point.y = ALL_GONE_COORDINATE; //Initialize y to zero
    for (int32_t j = ALIENS_NUM_ALIENS_HIGH-1; j >= 0; --j) //Start at the bottom and go up
    {
        if (alienArmy[i][j].stance <= alien_in) //If the stance indicates the alien is alive
        {
            //Set the x and y coordinates to the bottom middle of this chosen alien
            point.x = (uint32_t) (alienArmy[i][j].x + IMAGE_RENDER_ALIEN_WIDTH*IMAGE_RENDER_ALIEN_MAGNIFY*HALF);
            point.y = alienArmy[i][j].y + IMAGE_RENDER_ALIEN_HEIGHT*IMAGE_RENDER_ALIEN_MAGNIFY;
            break;
        }
    }
    return point;
}
