#ifndef IMAGE_RENDER_H
#define IMAGE_RENDER_H

#include "../control/control.h"
#include "../aliens/aliens.h"
#include "../video/video.h"
#include <stdint.h>

#define IMAGE_RENDER_BACKGROUND_COLOR 0x000000 //Black

#define IMAGE_RENDER_ALIEN_WIDTH 14             // Pixel width of alien
#define IMAGE_RENDER_ALIEN_HEIGHT 10            // Pixel height of alien
#define IMAGE_RENDER_ALIEN_MAGNIFY 2            // How much the alien is magnified
#define IMAGE_RENDER_ALIEN_COLOR 0xFFFFFF       // Aliens are white

#define IMAGE_RENDER_ALIEN_ARMY_OFFSET 16       // How many pixels are between the aliens
#define IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH (IMAGE_RENDER_ALIEN_WIDTH*IMAGE_RENDER_ALIEN_MAGNIFY+IMAGE_RENDER_ALIEN_ARMY_OFFSET) // Pixels of alien and space
#define IMAGE_RENDER_ALIEN_ARMY_BLOCK_HEIGHT (IMAGE_RENDER_ALIEN_HEIGHT*IMAGE_RENDER_ALIEN_MAGNIFY+IMAGE_RENDER_ALIEN_ARMY_OFFSET) // Pixels of alien and space
#define IMAGE_RENDER_ALIEN_ARMY_PIXEL_WIDTH (ALIENS_NUM_ALIENS_WIDE * IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH) // Pixels of whole alien army width
#define IMAGE_RENDER_ALIEN_ARMY_PIXEL_HEIGHT (ALIENS_NUM_ALIENS_HIGH * IMAGE_RENDER_ALIEN_ARMY_BLOCK_HEIGHT) // Pixels of whole alien army height

#define IMAGE_RENDER_ALIEN_BULLET_WIDTH 3       // Pixel width of alien bullet
#define IMAGE_RENDER_ALIEN_BULLET_HEIGHT 7      // Pixel height of alien height
#define IMAGE_RENDER_ALIEN_BULLET_COLOR 0xFEFEFE// Alien bullet color

#define IMAGE_RENDER_SAUCER_WIDTH 18            // Pixel width of saucer
#define IMAGE_RENDER_SAUCER_HEIGHT 7            // Pixel height of saucer
#define IMAGE_RENDER_SAUCER_COLOR 0xFF0000      // Color of saucer

#define IMAGE_RENDER_TANK_WIDTH 17              // Pixel width of tank
#define IMAGE_RENDER_TANK_HEIGHT 8              // Pixel height of tank
#define IMAGE_RENDER_TANK_MAGNIFY 2             // How much the tank is magnified
#define IMAGE_RENDER_TANK_COLOR 0x00FE00        // Tank color (Shade off of green to be unique for detecting hits)

#define IMAGE_RENDER_TANK_BULLET_WIDTH 1        // Pixel width of tank bullet
#define IMAGE_RENDER_TANK_BULLET_HEIGHT 7       // Pixel height of tank bullet
#define TANK_BULLET_MAGNIFY IMAGE_RENDER_TANK_MAGNIFY   // How much the tank bullet is magnified
#define IMAGE_RENDER_TANK_BULLET_COLOR 0xFFFFFF // Bullet is white

#define IMAGE_RENDER_BUNKER_WIDTH 24            // Pixel width of bunker
#define IMAGE_RENDER_BUNKER_HEIGHT 18           // Pixel height of bunker
#define IMAGE_RENDER_BUNKER_MAGNIFY 2           // How much the bunker is magnified
#define IMAGE_RENDER_BUNKER_COLOR 0x00FD00      // Bunker color (Shade off of green to be unique for detecting hits)
#define IMAGE_RENDER_BUNKER_PORTION_WIDTH 6     // Pixel width of bunker portion
#define IMAGE_RENDER_BUNKER_PORTION_HEIGHT 6    // Pixel height of bunker portion

#define IMAGE_RENDER_BUNKER_0_X (VIDEO_SCREEN_WIDTH*0.2-(IMAGE_RENDER_BUNKER_WIDTH*IMAGE_RENDER_BUNKER_MAGNIFY)/2) // X position of the 1st bunker
#define IMAGE_RENDER_BUNKER_1_X (VIDEO_SCREEN_WIDTH*0.4-(IMAGE_RENDER_BUNKER_WIDTH*IMAGE_RENDER_BUNKER_MAGNIFY)/2) // X position of the 2nd bunker
#define IMAGE_RENDER_BUNKER_2_X (VIDEO_SCREEN_WIDTH*0.6-(IMAGE_RENDER_BUNKER_WIDTH*IMAGE_RENDER_BUNKER_MAGNIFY)/2) // X position of the 3rd bunker
#define IMAGE_RENDER_BUNKER_3_X (VIDEO_SCREEN_WIDTH*0.8-(IMAGE_RENDER_BUNKER_WIDTH*IMAGE_RENDER_BUNKER_MAGNIFY)/2) // X position of the 4th bunker
#define IMAGE_RENDER_BUNKER_Y   (VIDEO_SCREEN_HEIGHT-IMAGE_RENDER_BUNKER_HEIGHT*IMAGE_RENDER_BUNKER_MAGNIFY*2 - (IMAGE_RENDER_TANK_HEIGHT*IMAGE_RENDER_TANK_MAGNIFY)) // Y coordinate of the bunkers

#define IMAGE_RENDER_SPRITE_WIDTH 5             // Pixel width of sprites (alphanumeric)
#define IMAGE_RENDER_SPRITE_HEIGHT 5            // Pixel height of sprites
#define IMAGE_RENDER_SPRITE_MAGNIFY 2           // How much they are magnified

// Render the top bar, bunkers and bottom lines
void image_render_init();

// Render the saucer at a certain x, y coordinate (x, y is top left corner of object)
void image_render_saucer(uint32_t x, uint32_t y);

// Render the saucer erased at a certain x, y coordinate (x, y is top left corner of object)
void image_render_saucerGone(uint32_t x, uint32_t y);

// Render an alien at a certain x, y coordinate (x, y is top left corner of object)
void image_render_singleAlien(uint32_t x, uint32_t y, const uint32_t *alien);

// Render the alien army at a certain x, y coordinate (x, y is top left corner of object)
void image_render_alienArmy(int32_t x, uint32_t y,
                            struct Alien alienArmy[ALIENS_NUM_ALIENS_WIDE][ALIENS_NUM_ALIENS_HIGH]);

// Render an alien bullet at a certain x, y coordinate (x, y is top left corner of object)
void image_render_alienBullet(uint32_t x, uint32_t y, const uint32_t *bulletType);

// Render the tank at a certain x, y coordinate (x, y is top left corner of object)
void image_render_tank(uint32_t x, uint32_t y, const uint32_t* tankState);

// Render the tank bullet at a certain x, y coordinate (x, y is top left corner of object)
void image_render_tankBullet(uint32_t x, uint32_t y);

// Render the tank bullet erased at a certain x, y coordinate (x, y is top left corner of object)
void image_render_tankBulletGone(uint32_t x, uint32_t y);

// Render a bunker at a certain x, y coordinate (x, y is top left corner of object)
void image_render_bunker(uint32_t x, uint32_t y);

// Render a bunker portion at a certain x, y coordinate (x, y is top left corner of object)
void image_render_bunkerPortion(uint32_t x, uint32_t y, const uint32_t *bunkerPortion);

// Render the score and lives
void image_render_topBar();

// Render the score at a certain x, y coordinate (x, y is top left corner of object)
// Used for top bar and high scores
void image_render_score(uint32_t x, uint32_t y, uint32_t score);

// Redraw the score
void image_render_updateScore();

// Redraw the lives
void image_render_lives(uint8_t num_lives);

// Render the game over screen
void image_render_gameOver();

// Render the phrase enter your name
void image_render_enterName(uint32_t rgbValue, const char *initials);

// Render the high scores
void image_render_highScores(const struct HighScore *highScores);


#endif // IMAGE_RENDER_H
