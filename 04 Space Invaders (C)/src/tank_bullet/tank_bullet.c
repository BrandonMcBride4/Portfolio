#include "tank_bullet.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../btns/btns.h"
#include "../globals/globals.h"
#include "../tank/tank.h"
#include "../saucer/saucer.h"
#include "../bunkers/bunkers.h"
#include "../image_render/image_render.h"
#include "../video/video.h"
#include "../user_audio/user_audio.h"

#define SHOOT_BUTTON 1 //The right most button will shoot a bullet from the tank
#define BULLET_DISTANCE_PER_TICK 3 //The number of pixels that the bullet moves every clock tick (If it exceeds three you must change the bullet sprite to have a larger buffer)
#define INITIAL_Y_BULLET_POSITION TANK_Y_COORDINATE - IMAGE_RENDER_TANK_BULLET_HEIGHT * 2 //The position that the tank bullet starts at in the Y axis
#define TOP_OF_SCREEN 30 //Pixel value of the top of the screen
#define BULLET_DETECTION_OFFSET 2 //Additional detection reach we want to bullet to be capable of

static uint32_t bullet_x_pos = 0; //Keeps track of the current bullet x position
static uint32_t bullet_y_pos = 0; //Keeps track of the current bullet y position
static bool isEnabled; // Boolean indicating if the state machine is enabled
static bool hasCollided = false; //Is true if a bulet collision has occurred

static enum tank_bullet_st_t
{
    init_st, //State machine is initialized in this state
    rest_st, //State representing the buttet when it is not being shot
    shoot_st //State representing the bullet when it is being shot
} currentState;

void tank_bullet_init() //Initialize the tank bullet state machine
{
    currentState = init_st; //Initialize the state machine to the init state
}

void tank_bullet_enable() //Enable the tank bullet state machine
{
    isEnabled = true; //Set to enabled
}

void tank_bullet_disable() //Disable the tank bullet state machine
{
    isEnabled = false; //Set to not enabled
}

void processCollision(uint32_t x, uint32_t y, uint32_t collisionColor) //Process a collision of the bullet with something else
{
    uint32_t mostBottomAlienY = 0; //The y coordinate of the bottom most alien`
    uint32_t columnBottomAlienY = 0; //The most bottom alien of a certain column
    // Gets the y coordinate of the bottom most alien of the whole army
    for (int i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i) //Iterate through the aliens to get the bottom most alien
    {
        columnBottomAlienY = aliens_getBottomMiddleOfAlien(i).y; //Get the bottom middle alien of the column
        if (columnBottomAlienY > mostBottomAlienY) //If the bottom alien of the column is further down than the curret most bottom alien`
        {
            mostBottomAlienY = columnBottomAlienY; // Set the most bottom alien column to that column
        }
    }
    switch (collisionColor) // We can detect what was being hit based on the color of the pixel that was hit
    {
        case IMAGE_RENDER_BUNKER_COLOR: //A bunker was hit
            process_bunker_collision(x, y);
            break;
        case IMAGE_RENDER_SAUCER_COLOR: //A saucer was hit
            saucer_hit();
            break;
        case IMAGE_RENDER_ALIEN_COLOR: //An alien was hit
            aliens_hit(x, y);
            break;
        case IMAGE_RENDER_ALIEN_BULLET_COLOR: //An alien bullet was hit
            break;
        default:
            printf("Invalid detection color %X detected from alien bullet\n", collisionColor); //None of the valid collisions were detected
    }
}


void tank_bullet_tick() // Tank bullet updates to be processed every timer interval
{
    //state update
    switch (currentState)
    {
        case init_st: //State machine is initialized in this state
            if (isEnabled)
            {
                currentState = rest_st; // Start state machine
            }
            else
            {
                currentState = init_st; //Stay in init state until bullets are enabled
            }
            break;
        case rest_st: //State representing the bullet when it is not being shot
            if (!isEnabled)
            {
                currentState = init_st; // If not enabled go to init
            }
            // If the right button is being pressed (with any other combination of buttons)
            else if (globals_getBtnValue() & SHOOT_BUTTON) //If the shoot button is pressed
            {
                currentState = shoot_st; //transition to shoot state
                bullet_x_pos = tank_getMidTankX(); //set the bullet x position to the middle of the tank
                bullet_y_pos = INITIAL_Y_BULLET_POSITION; //Set the bullet y position
                hasCollided = false; //Reset the collition indicator
                user_audio_playLaser(); // Make laser sound
            }
            else
            {
                currentState = rest_st; //Remain on this state
            }
            break;
        case shoot_st: //State representing the bullet when it is being shot
            if (!isEnabled)
            {
                currentState = init_st; // If not enabled go to init
            }
            else if (bullet_y_pos <= TOP_OF_SCREEN || hasCollided) //If the bullet has reached the top of the screen or if it has collided into something
            {
                image_render_tankBulletGone(bullet_x_pos, bullet_y_pos); //Erase the bullet
                currentState = rest_st; //Go back to rest state
            }
            break;
        default:
            printf("tank_bullet_tick state update hit default");
            break;
    }

    //state action
    switch (currentState)
    {
        case init_st: //State machine is initialized in this state
            break;
        case rest_st: //State representing the bullet when it is not being shot
            break;
        case shoot_st: //State representing the bullet when it is being shot
            bullet_y_pos = bullet_y_pos - BULLET_DISTANCE_PER_TICK; //Decrement the y position every cycle
            image_render_tankBullet(bullet_x_pos, bullet_y_pos); //Redraw the bullet at the appropriate position
            int32_t bullet_read_y = bullet_y_pos-1; //Read ahead a pixel to predict a collition
            uint32_t readBulletColor = 1; //This reads the pixel that the bullet will be traveling to
            for (int32_t i = 0; i <= IMAGE_RENDER_TANK_BULLET_WIDTH * IMAGE_RENDER_TANK_MAGNIFY + BULLET_DETECTION_OFFSET; ++i) //Cycle through the horizontal range of the bullet
            {
                readBulletColor = video_readPixel(bullet_x_pos + i, bullet_read_y); //Read the pixel color of the upcoming bullet position
                if (readBulletColor != IMAGE_RENDER_BACKGROUND_COLOR) // If the bullet will hit something other than the background
                {
                    processCollision(bullet_x_pos + i, bullet_read_y, readBulletColor); //Process what the collision was and where it was
                    hasCollided = true; //Set the collision indicator to true
                    break; //Don't check for collision anymore with this bullet
                }
            }
            break;
        default:
            printf("tank_bullet_tick state action hit default");
            break;
    }
}
