#ifndef TANK_H
#define TANK_H
#include <stdint.h>
#include "../image_render/image_render.h"
#include "../video/video.h"

#define TANK_Y_COORDINATE (VIDEO_SCREEN_HEIGHT-(IMAGE_RENDER_TANK_WIDTH)-7) //The y coordinate of the tank relative to the top of the screen


void tank_init();

void tank_tick();

void tank_hit();

uint32_t tank_getMidTankX();

void tank_enable();

void tank_disable();

#endif // TANK_H
