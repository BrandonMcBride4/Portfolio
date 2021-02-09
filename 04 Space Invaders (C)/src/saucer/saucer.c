#include "saucer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../video/video.h"
#include "../globals/globals.h"
#include "../image_render/image_render.h"
#include "../user_audio/user_audio.h"

#define SAUCER_RIGHT_LIMIT VIDEO_SCREEN_WIDTH - 40 //The furthest right position that the saucer travels in pixels
#define SAUCER_LEFT_LIMIT 10 //The furthest left position that the saucer travels in pixels
#define SAUCER_TICKS_PER_INCREMENT 1 //The number of ticks that pass before the saucer moves another increment
#define TICKS_PER_SECOND 100 //Number of clock ticks that compose one second
#define MIN_SAUCER_INTERVAL 10 * TICKS_PER_SECOND //Minimum number of ticks that must pass before another saucer passes
#define MAX_SAUCER_INTERVAL 30 * TICKS_PER_SECOND //Maximum number of seconds that can pass before another saucer passes
#define RESET 0 //Used to reset counters
#define NUMBER_OF_DIRECTIONS 2 //Number of directions in which the saucer can come from
#define SAUCER_POINTS_MULTIPLE 50 //When a saucer it hit, the score is incremented in multiples of this macro
#define SAUCER_POINTS_VARIANT 7 //When a sacuer is hit, this many increments of the multiple are possible

static uint32_t saucer_x_pos = 0; //Keeps track of the current saucer position
static uint32_t random_saucer_interval = 0; //Randomly assigned interval between 10 and 30 seconds representing the time before the next saucer flies by
static uint32_t saucer_interval_counter = 0; //Counter that keeps track of time passed since the last saucer
static uint32_t saucer_ticks_since_increment = 0; //Keeps track of the number of clock ticks that have passsed since the last time the saucer incremented a pixel
static bool isEnabled; //Boolean representing whether the saucer state machine is enabled

uint32_t get_random_saucer_interval() // Returns random interval between 10 and 30 seconds
{
    return (rand() % (MAX_SAUCER_INTERVAL - MIN_SAUCER_INTERVAL) + MIN_SAUCER_INTERVAL);
}

static enum saucer_st_t
{
    init_st, //Starting state
    no_saucer_st, //Waits for saucer interval before sending a new saucer
    assign_direction_st, //Assigns a random direction for the saucer to move (left or right)
    move_left_st, //Moves saucer left across the screen
    move_right_st //Moves saucer right across the screen
} currentState;

void saucer_init() //Initialize the saucer state machine
{
    saucer_enable(); //Enable the saucer
    currentState = init_st; //Start the state machine at the init state
}

void saucer_enable() //enable the saucer state machine
{
    isEnabled = true;
}

void saucer_disable() //Disable the sacuer state machine
{
    isEnabled = false;
}

// enables UFO sound and plays the sound if nothing else isn't playing
void playUFOSound()
{
    if (!user_audio_isPlaying()) // Just in case other sounds play like shooting
    {
        user_audio_playUFO();    // Play UFO sound
    }
}

void saucer_tick() //This is updated every tick cycle
{

    //state update
    switch (currentState)
    {
        case init_st: //Starting state
            if (isEnabled)
            {
                currentState = no_saucer_st; //Start the state machine at the no saucer state
            }
            else
            {
                currentState = init_st; //If the state machine is disabled, stay in init state
            }
            break;
        case no_saucer_st: //Waits for saucer interval before sending a new saucer
            if (!isEnabled)
            {
                currentState = init_st; //If the state machine is disabled, go to init state
            }
            else if (saucer_interval_counter >= random_saucer_interval) //If the randomly set counter has expired
            {
                currentState = assign_direction_st; //Set the current state to the sttate that assigns a random direction
            }
            break;
        case assign_direction_st: //Assigns a random direction for the saucer to move (left or right)
            if (!isEnabled)
            {
                currentState = init_st; //If the state machine is disabled, go to init state
            }
            else if (rand() % NUMBER_OF_DIRECTIONS)   //randomly assign the saucer to move left or right
            {
                currentState = move_left_st; // You have been randomly selected to move left!
                image_render_saucer(SAUCER_RIGHT_LIMIT, SAUCER_Y_COORDINATE); // Start from the right
                saucer_x_pos = SAUCER_RIGHT_LIMIT; //Set the x position to the right most part of the screen
                saucer_ticks_since_increment = RESET; //Reset the counter representing the number of ticks since it increments
            }
            else
            {
                currentState = move_right_st; // You have been randomly selected to move right!
                image_render_saucer(SAUCER_LEFT_LIMIT, SAUCER_Y_COORDINATE); // Start from the left
                saucer_x_pos = SAUCER_LEFT_LIMIT; //Set the x position to the left most part of the screen
                saucer_ticks_since_increment = RESET; //Reset the counter representing the number of ticks since it increments
            }
            break;
        case move_left_st: //Moves saucer left across the screen
            if (!isEnabled)
            {
                currentState = init_st; //If the state machine is disabled, go to init state
            }
            else if (saucer_x_pos < SAUCER_LEFT_LIMIT) //If the saucer flies off the screen
            {
                currentState = no_saucer_st; //Return to home state
                image_render_saucerGone(saucer_x_pos, SAUCER_Y_COORDINATE); //Erase the saucer
                random_saucer_interval = get_random_saucer_interval(); //get a new random interval to wait for next saucer
                saucer_interval_counter = RESET; //reset counter in preparation to count again
            }
            break;
        case move_right_st: //Moves saucer right across the screen
            if (!isEnabled)
            {
                currentState = init_st; //If the state machine is disabled, go to init state
            }
            else if (saucer_x_pos > SAUCER_RIGHT_LIMIT) //If the saucer hits the end of the screen
            {
                currentState = no_saucer_st; //Return to the home state
                image_render_saucerGone(saucer_x_pos, SAUCER_Y_COORDINATE); //Erase the saucer
                random_saucer_interval = get_random_saucer_interval(); //get a new random interval to wait for next saucer
                saucer_interval_counter = RESET; //reset counter in preparation to count again
            }
            break;
        default:
            printf("saucer_tick state update hit default");
            break;
    }

    //state action
    switch (currentState)
    {
        case init_st: //Starting state
            user_audio_disableUFOSound(); // No saucer so play background music
            break;
        case no_saucer_st: //Waits for saucer interval before sending a new saucer
            user_audio_disableUFOSound(); // No saucer so play background music
            saucer_interval_counter++; //Increment the interval counter
            break;
        case assign_direction_st: //Assigns a random direction for the saucer to move (left or right)
            user_audio_disableUFOSound(); // No saucer so play background music
            break;
        case move_left_st: //Moves saucer left across the screen
            playUFOSound(); // UFO present so play the sound
            saucer_ticks_since_increment++; //Increment the ticks since incement timer
            if (saucer_ticks_since_increment == SAUCER_TICKS_PER_INCREMENT) //If sufficeint delay has passed,
            {
                image_render_saucer(--saucer_x_pos, SAUCER_Y_COORDINATE); //Redraw the saucer to the left
                saucer_ticks_since_increment = RESET; //Reset the ticks since increment timer
            }
            break;
        case move_right_st: //Moves saucer right across the screen
            playUFOSound(); // UFO present so play the sound
            saucer_ticks_since_increment++; //Increment the ticks since incement timer
            if (saucer_ticks_since_increment == SAUCER_TICKS_PER_INCREMENT)
            {
                image_render_saucer(++saucer_x_pos, SAUCER_Y_COORDINATE); //Redraw the saucer to the right
                saucer_ticks_since_increment = RESET; //Reset the ticks since increment timer
            }
            break;
        default:
            printf("saucer_tick state action hit default");
            break;
    }
}

void saucer_hit() // Sequence to go through when the saucer is hit
{
    user_audio_playUFODie(); // Play sound
    currentState = no_saucer_st; //Return to the home state
    image_render_saucerGone(saucer_x_pos, SAUCER_Y_COORDINATE); //Erase the saucer
    random_saucer_interval = get_random_saucer_interval(); //get a new random interval to wait for next saucer
    saucer_interval_counter = RESET; //reset counter in preparation to count again
    int32_t saucer_points = SAUCER_POINTS_MULTIPLE * (rand() % SAUCER_POINTS_VARIANT); //Randomly generate a score between 50 and 350 to add to your total score
    globals_incScore(saucer_points); //Increment the global score count
    image_render_updateScore(); //Update the score on the screen
}
