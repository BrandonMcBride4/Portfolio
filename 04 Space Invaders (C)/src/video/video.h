#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

/*********************************** macros ***********************************/
#define VIDEO_SCREEN_WIDTH 640  // The screen is 640 pixels wide
#define VIDEO_SCREEN_HEIGHT 480 // The screen is 480 pixels long
/**************************** function prototypes *****************************/

// initializes the uio driver
int32_t video_init(char devDevice[]);

// Write a pixel at a coorinate with a certain color
void video_writePixel(uint32_t x, uint32_t y, uint32_t rgbValue);

// Writes a line with a bitmask at a x, y
// 0 is background, 1 is color given
void video_writeMaskedLine(uint32_t x, uint32_t y, uint32_t rgbValue, uint32_t width, uint32_t mask, uint32_t mag);

// Writes a line at given location
void video_writeHLine(uint32_t x, uint32_t y, uint32_t rgbValue, uint32_t width);

// Writes the screen one color
void video_writeScreen(uint32_t rgbValue);

// Reads the screen at a location and reads the pixel color
uint32_t video_readPixel(uint32_t x, uint32_t y);

// Closes file descriptor
void video_exit();

#endif // VIDEO_H