#include <stdint.h>
#include <stdbool.h>

enum bulletType {ZIGZAG, CROSS}; //Different types of bullets
enum bulletStance {UP, DOWN}; //Different stances that the bullets can have

struct AlienBullet
{
    uint32_t x_pos; //Current x position of the bullet
    uint32_t y_pos; //Current y posiiton of the bullet
    enum bulletType type; //Type of bullet that is being shop
    enum bulletStance stance; //Current stance of the bullet that is being shot
    bool isActive; //Indicates whether the bullet is currently active
    bool hasCollided; //Indicate whether the bullet has collided with something
};

void alien_bullet_init();

void alien_bullet_tick();

void alien_bullet_enable();

void alien_bullet_disable();
