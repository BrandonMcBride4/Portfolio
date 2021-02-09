#include "alien_bullet.h"
#include <stdio.h>
#include <stdlib.h>
#include "../btns/btns.h"
#include "../globals/globals.h"
#include "../tank/tank.h"
#include "../image_render/image_render.h"
#include "../video/video.h"
#include "../image_render/sprite.h"
#include "../bunkers/bunkers.h"

#define BULLET_DISTANCE_PER_TICK 3 //The number of pixels that the bullet moves every clock tick (If it exceeds three you must change the bullet sprite to have a larger buffer)
#define BOTTOM_OF_SCREEN (480-18) //Pixel value of the point of th escreen that bullets will stop
#define TICKS_PER_SECOND 100 //Number of clock ticks that compose one second
#define ALIEN_BULLET_MAX_DELAY 2 * TICKS_PER_SECOND //Maximum number of ticks that can pass before another bullet is shot
#define ALIEN_BULLET_MAX_COUNT 4 //Maximum number of bullets that can exist at a time
#define ALIEN_COLUMN_COUNT 11 //Number of columns of aliens that there are
#define RESET 0 //Used to reset counters
#define NUM_BULLET_TYPES 2 //Number of types of bullets that there can be

static uint32_t counter = 0; //Counter used to keep track of how to
static uint32_t next_bullet_delay = 0; //counter representing the delay until the next bullet
static uint32_t number_of_bullets = 0; //represent the current number of active bullets
static struct AlienBullet bullets[ALIEN_BULLET_MAX_COUNT]; //Holds information for each of the alien bullets
static bool isEnabled; //Represents whether the alien bullets are enabed or not


void alien_bullet_init() //Initialize the alien bullets
{
    next_bullet_delay = rand() % ALIEN_BULLET_MAX_DELAY; //Get a delay interval between 0 and 2 seconds
    for (uint32_t i = 0; i < ALIEN_BULLET_MAX_COUNT; i++) //Iterate through al of the alien bullets
    {
        bullets[i].isActive = false; //Set to inactive
        bullets[i].hasCollided = false; //Set to not having collided
    }
    number_of_bullets = RESET; //Reset the number of bullets to zero
    counter = RESET; //Reset the counter to zero
}

void eraseAlienBullet(uint32_t x, uint32_t y) //Earse the alien bullet on the screen at the given coordinates
{
    image_render_alienBullet(x, y, alienbullet_gone_3x7);
}

void drawAlienBullet(uint32_t x, uint32_t y, enum bulletType type, enum bulletStance stance) //Draw a new alien bullet to the creen at the given coordinates and of the given type and stance
{
    if (type == ZIGZAG) //Draw a zigzag bullet
    {
        if (stance == UP) //If stance is up, draw it down
        {
            image_render_alienBullet(x, y, alienbullet1_up_3x7);
        }
        else if (stance == DOWN)  //If stance is down, draw it up
        {
            image_render_alienBullet(x, y, alienbullet1_down_3x7);
        }
    }
    else if (type == CROSS) //Draw a cross bullet
    {
        if (stance == UP)  //If stance is up, draw it down
        {
            image_render_alienBullet(x, y, alienbullet2_up_3x7);
        }
        else if (stance == DOWN) //If stance is down, draw it up
        {
            image_render_alienBullet(x, y, alienbullet2_down_3x7);
        }
    }
}

void incrementBullets() //Increment the number of bullets that there are
{
    for (uint32_t i = 0; i < ALIEN_BULLET_MAX_COUNT; i++) //Iterate through all of the bullets
    {
        if (bullets[i].isActive == false) //If the bullet is inactive, don't do anything
        {
            continue;
        }
        bullets[i].y_pos += BULLET_DISTANCE_PER_TICK; //Increment the y position of the alien bullet
        if (bullets[i].y_pos >= BOTTOM_OF_SCREEN || bullets[i].hasCollided) //If the bullet has reached the bottom of the screen or if it has collided with something
        {
            eraseAlienBullet(bullets[i].x_pos, bullets[i].y_pos); //Erase the alien bullet
            bullets[i].isActive = false; //Set the bullet to inactive
            number_of_bullets--; //decrement the number of bullets
            bullets[i].hasCollided = false; //Reset the collition indicator
        }
        else //Otherwise update the stance and draw the bullet
        {
            //Toggle the stance
            if (bullets[i].stance == UP)
            {
                bullets[i].stance = DOWN;
            }
            else //If the stance is down
            {
                bullets[i].stance = UP;
            }
            drawAlienBullet(bullets[i].x_pos, bullets[i].y_pos, bullets[i].type, bullets[i].stance);
        }
    }
}

void createNewBullet() //Create a new alien bullet
{
    uint32_t index; //Represents the index of the array that this new bullet will be placed in
    for (index = 0; index < ALIEN_BULLET_MAX_COUNT; index++) //Find an inactive index to fill with a new bullet
    {
        if (bullets[index].isActive == false) //If the bullet is inactive, use that index
        {
            break;
        }
    }

    uint32_t random_column = 0; //Variable to hold a randomly chosen column of aliens
    struct Point temp_p; //Point at which the bottom alien is located
    do //Iterate through columns until finding one with aliens in it
    {
        random_column = rand() % ALIEN_COLUMN_COUNT; //Find a random column
        temp_p = aliens_getBottomMiddleOfAlien(random_column); //get the point of the bottom most alien
        if (aliens_checkIfAllAliensAreHit()) //If all of the aliens have been hit, returns
        {
            return;
        }
    }
    while (!temp_p.x); // If the x is 0 then there are no aliens in that column

    bullets[index].x_pos = temp_p.x; //Initiale the x position fo the bullet
    bullets[index].y_pos = temp_p.y; //Initiale the y position fo the bullet
    bullets[index].stance = UP; //Initialize the stance of the bullet
    bullets[index].isActive = true; //Set the bullet to active

    if (rand() % NUM_BULLET_TYPES) //Pick a random bullet type
    {
        bullets[index].type = ZIGZAG;
    }
    else
    {
        bullets[index].type = CROSS;
    }
    number_of_bullets++; //Increment the counter representing the number of active bullets
    drawAlienBullet(bullets[index].x_pos, bullets[index].y_pos, bullets[index].type,
                    bullets[index].stance); //Draw the bullet at the initial position
    next_bullet_delay = rand() % ALIEN_BULLET_MAX_DELAY; //Get a random delay interval

}

void processAlienCollision(uint32_t x, uint32_t y, uint32_t collisionColor) //Process a collision of an alin bullet
{

    if (collisionColor == IMAGE_RENDER_BUNKER_COLOR) //Alien bullet detects a bunker color
    {
        process_bunker_collision(x, y);
    }
    else if (collisionColor == IMAGE_RENDER_TANK_COLOR) //Alien bullet detects a tank color
    {
        tank_hit();
    }
}

void alien_bullet_tick() //Update alien bullets
{
    if (isEnabled) //Only do this if the alien bullets are enabled
    {
        counter++; //Increment the counter
        if (counter >= next_bullet_delay && number_of_bullets < ALIEN_BULLET_MAX_COUNT &&
            !aliens_checkIfAllAliensAreHit()) //If the counter indicates that its time for a new bullet and the maximum number of bullets has not been reached
        {
            counter = RESET; //Reset the counter
            createNewBullet(); //Create a new bullet
        }
        incrementBullets(); //Increment the bullet positions

        for (int32_t j = 0; j < ALIEN_BULLET_MAX_COUNT; j++)
        {
            if (bullets[j].isActive)
            {
                int32_t readBulletColor; // Color that the bullet is reading one ahead
                int32_t readBullet_y;    // The y position the bullet is reading
                for (int32_t i = 0; i <= IMAGE_RENDER_ALIEN_BULLET_WIDTH * IMAGE_RENDER_ALIEN_MAGNIFY + 1; ++i) //Iterate through the width of the alien bullet
                {
                    readBullet_y = bullets[j].y_pos + IMAGE_RENDER_ALIEN_BULLET_HEIGHT * IMAGE_RENDER_ALIEN_MAGNIFY + 1; //Read the y position of the
                    readBulletColor = video_readPixel(bullets[j].x_pos + i, readBullet_y); //Read the color that the bullet is going to travel onto
                    if (readBulletColor != IMAGE_RENDER_BACKGROUND_COLOR) //If the bullet is going to travel past anytihng but the background, process a collision
                    {
                        processAlienCollision(bullets[j].x_pos + i, readBullet_y, readBulletColor); //Process a vollision at the give x and y coordinates
                        bullets[j].hasCollided = true; //Set the collision indicator to true
                        break;
                    }
                }
            }
        }
    }
}

void alien_bullet_enable() //Enable alien bullets
{
    isEnabled = true;
}

void alien_bullet_disable() //Disable alien bullets
{
    isEnabled = false;
}
