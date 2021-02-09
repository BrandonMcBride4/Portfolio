#include "tank.h"
#include <stdio.h>
#include <stdbool.h>
#include "../btns/btns.h"
#include "../globals/globals.h"
#include "../video/video.h"
#include "../image_render/sprite.h"
#include "../user_audio/user_audio.h"

#define LEFT_BUTTON 8 //The left most button will move the tank right
#define RIGHT_BUTTON 4 //The second button to the left will move the tank right
#define TANK_RIGHT_LIMIT 600 //The furthest right position that the tank can travel in pixels
#define TANK_LEFT_LIMIT 10 //The furthest left position that the tank can travel in pixels
#define TANK_STARTING_POSITION (IMAGE_RENDER_BUNKER_0_X + IMAGE_RENDER_BUNKER_WIDTH)
#define TANK_DISTANCE_PER_TICK 2 //The number of pixels that the tank moves every clock tick
#define RESET 0 //Value used to reset the counter
#define DEATH_TRANSITIONS 10 //Number of flickers that the tank goes through when it dies
#define TANK_BULLET_OFFSET 3 //Offset left for placing the tank bullet to the middle of the tank
#define HALF 0.5 //Macro for splitting a value in half

static bool isEnabled; //Bollean to indicate if the state machine is enabled
static uint32_t tank_x_pos = 0; //Keeps track of the current tank position
static uint32_t death_counter = 0; //Keeps track of how many times the tank runs through the death cycle
static bool tankHit = false; //Boolean to determine if a tank has been hit

static enum tank_st_t
{
    init_st, //State machine is initialized in this state
    rest_st, //State representing the tank when it is not moving
    move_left_st, //State for when the tank is moving left
    move_right_st, //State for when the tank is moving right
    dead1_st, //State for when the tank dies
    dead2_st //State representing the other image that is toggled when the tank dies
} currentState;

void tank_init() //Initailize the tank state machine and parameters relating to it
{
    tank_enable(); //Enable the tank state machine
    currentState = init_st; //Set the starting state to init state
    tank_x_pos = TANK_STARTING_POSITION; //Set the x position of the tank to the starting position on the screen
    image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_17x8); //Draw the tank to the starting position of the screen
    death_counter = RESET; //Reset the death counter to zero
}

//Returns the x position of the center of the tank
uint32_t tank_getMidTankX()
{
    return tank_x_pos + IMAGE_RENDER_TANK_WIDTH * IMAGE_RENDER_TANK_MAGNIFY * HALF - IMAGE_RENDER_TANK_BULLET_WIDTH * IMAGE_RENDER_TANK_MAGNIFY - TANK_BULLET_OFFSET; //Add half of the tank width to the current x position which corresponds to the left corner of the tank
}

void tank_enable() //Enable the tank state machine
{
    isEnabled = true;
}

void tank_disable() //Disable the tank state machine
{
    isEnabled = false;
}

void tank_hit() //Set the tank as hit
{
    user_audio_playPlayerDie(); // Play sound
    tankHit = true;
}

void tank_tick() //State machine for the tank
{
    //state update
    switch (currentState)
    {
        case init_st: //State machine is initialized in this state
            if (isEnabled)
            {
                currentState = rest_st; // Start the state machine in the rest state
            }
            else
            {
                currentState = init_st; // If not enabled go to init
            }
            break;
        case rest_st: //State representing the tank when it is not moving
            if (!isEnabled)
            {
                currentState = init_st; // If not enabled go to init
            }
            else if (tankHit) //If the tank has been hit
            {
                currentState = dead1_st; //Tranistion to the dead state
                death_counter = RESET; //Reset the death counter in anticipation of incrementing it
                tankHit = false; //Reset the tank hit indicator
            }
            else if (globals_getBtnValue() & LEFT_BUTTON) //If the left button is being pressed (with any other combination of buttons)
            {
                currentState = move_left_st; //Start moving left
            }
            else if ( globals_getBtnValue() & RIGHT_BUTTON) //If the right button is being pressed (with any other combination of buttons)
            {
                currentState = move_right_st; //Start moving right
            }
            else
            {
                currentState = rest_st; //Stay in rest state
            }
            break;
        case move_left_st: //State for when the tank is moving left
            if (!isEnabled)
            {
                currentState = init_st; // If not enabled go to init
            }
            else if (tankHit) //If the tank has been hit
            {
              currentState = dead1_st; //Tranistion to the dead state
              death_counter = RESET; //Reset the death counter in anticipation of incrementing it
              tankHit = false; //Reset the tank hit indicator
            }
            else if (!( globals_getBtnValue() & LEFT_BUTTON))  //If the left button is not being pressed
            {
                currentState = rest_st; //Stop moving
            }
            else
            {
                currentState = move_left_st; //Keep moving left
            }
            break;
        case move_right_st: //State for when the tank is moving right
            if (!isEnabled)
            {
                currentState = init_st; // If not enabled go to init
            }
            else if (tankHit) // If the tank was hit
            {
              currentState = dead1_st; //Tranistion to the dead state
              death_counter = RESET; //Reset the death counter in anticipation of incrementing it
              tankHit = false; //Reset the tank hit indicator
            }
            else if (!( globals_getBtnValue() & RIGHT_BUTTON)) //If the right button is not being pressed
            {
                currentState = rest_st; //Stop moving
            }
            else
            {
                currentState = move_right_st; //Keep moving right
            }
            break;
        case dead1_st: //State for when the tank dies
            if (death_counter < DEATH_TRANSITIONS) { //If the number of cycles through the death sequence is below the threshold
                currentState = dead2_st; //Render the next death flash
            }
            else //If the number of cycles through the death sequence is above the threshold
            {
                image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_gone_17x8); //Erase the tank
                tank_init(); //Reinitialize the tank
                globals_decLives(); //Decrement the number of lives
            }
            break;
        case dead2_st: //State representing the other image that is toggled when the tank dies
            if (death_counter < DEATH_TRANSITIONS) {
                currentState = dead1_st; //Render the next death flash
            }
            else //If the number of cycles through the death sequence is above the threshold
            {
                image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_gone_17x8); //Erase the tank
                tank_init(); //Reinitialize the tank
                globals_decLives(); //Decrement the number of lives
            }
            break;
        default:
            printf("tank_tick state update hit default");
            break;
    }

    //state action
    switch (currentState)
    {
        case init_st: //State machine is initialized in this state
            break;
        case rest_st: //State representing the tank when it is not moving
            break;
        case move_left_st: //State for when the tank is moving left
            if (tank_x_pos > TANK_LEFT_LIMIT)
            { //If tank is not on the leftmost part of the screen
                tank_x_pos = tank_x_pos - TANK_DISTANCE_PER_TICK; //Decrement the position marker
                image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_17x8); //Draw the new tank image at the new position
            }
            break;
        case move_right_st: //State for when the tank is moving right
            if (tank_x_pos < TANK_RIGHT_LIMIT)
            { //If tank is not on the rightmost part of the screen
                tank_x_pos = tank_x_pos + TANK_DISTANCE_PER_TICK;  //Increment the position marker
                image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_17x8); //Draw the new tank image at the new position
            }
            break;
        case dead1_st: //State for when the tank dies
            image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_gone_17x8); //Erase the tank
            image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_explosion1_17x8); //Erase the tank
            break;
        case dead2_st: //State representing the other image that is toggled when the tank dies
            image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_gone_17x8); //Erase the tank
            image_render_tank(tank_x_pos, TANK_Y_COORDINATE, tank_explosion2_17x8); //Erase the tank
            death_counter++; //Increment the death counter
            break;
        default:
            printf("tank_tick state action hit default");
            break;
    }
}
